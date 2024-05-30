#include "ncurses_display.h"
#include "linux_parser.h"
#include <curses.h>

#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include <map>

#include "format.h"
#include "system.h"

using std::string;
using std::to_string;

template <typename T>
void clearColumn(T window, int row, std::map<std::string, int>& columns, const std::string& column_name) {
    std::string clear_string(columns[column_name], ' ');
    mvwprintw(window, row, columns[column_name], "%s", clear_string.c_str());
}

// 50 bars uniformly displayed from 0 - 100 %
// 2% is one bar(|)
std::string NCursesDisplay::ProgressBar(float percent) {
  std::string result{"0%"};
  int size{50};
  float bars{percent * size};

  for (int i{0}; i < size; ++i) {
    result += i <= bars ? '|' : ' ';
  }

  string display{to_string(percent * 100).substr(0, 4)};
  if (percent < 0.1 || percent == 1.0)
    display = " " + to_string(percent * 100).substr(0, 3);
  return result + " " + display + "/100%";
}

void NCursesDisplay::DisplaySystem(System& system, WINDOW* window) {
  int row{0};
  mvwprintw(window, ++row, 2, "OS: %s", system.OperatingSystem().c_str());
  mvwprintw(window, ++row, 2, "Kernel: %s", system.Kernel().c_str());
  mvwprintw(window, ++row, 2, "CPU: ");
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, row, 10, "%s", "");
    wprintw(window, "%s", ProgressBar(system.Cpu().Utilization()).c_str());
    wattroff(window, COLOR_PAIR(1));
    mvwprintw(window, ++row, 2, "Memory: ");
    wattron(window, COLOR_PAIR(1));
    mvwprintw(window, row, 10, "%s", "");
    wprintw(window, "%s", ProgressBar(system.MemoryUtilization()).c_str());
    wattroff(window, COLOR_PAIR(1));
    mvwprintw(window, ++row, 2, "Total Processes: %d", system.TotalProcesses());
    mvwprintw(
        window, ++row, 2,
        "Running Processes: %d", system.RunningProcesses());
    mvwprintw(window, ++row, 2,
              "Up Time: %s", Format::ElapsedTime(system.UpTime()).c_str());
  wrefresh(window);
}

void NCursesDisplay::DisplayProcesses(std::vector<Process>& processes,
                                      WINDOW* window, int n) {
  int row{0};
  std::map<std::string, int> columns = {
    {"pid_column", 2},
    {"user_column", 10},
    {"cpu_column", 19},
    {"ram_column", 26},
    {"time_column", 35},
    {"command_column", 46}
  };

  wattron(window, COLOR_PAIR(2));
  mvwprintw(window, ++row, columns["pid_column"], "PID");
  mvwprintw(window, row, columns["user_column"], "USER");
  mvwprintw(window, row, columns["cpu_column"], "CPU[%%]");
  mvwprintw(window, row, columns["ram_column"], "RAM[MB]");
  mvwprintw(window, row, columns["time_column"], "TIME+");
  mvwprintw(window, row, columns["command_column"], "COMMAND");

  wattroff(window, COLOR_PAIR(2));
  int const num_processes = int(processes.size()) > n ? n : processes.size();
  for (int i = 1; i < num_processes + 1; ++i) {

    mvwprintw(window, ++row, columns["pid_column"], "%d", processes[i].Pid());
    clearColumn(window, row, columns, "user_column");
    mvwprintw(window, row, columns["user_column"], "%s", processes[i].User().c_str());
    clearColumn(window, row, columns, "cpu_column");
    float cpu = processes[i].CpuUtilization() * 100;
    mvwprintw(window, row, columns["cpu_column"], "%.2f", cpu);
    clearColumn(window, row, columns, "ram_column");
    mvwprintw(window, row, columns["ram_column"], "%s", processes[i].Ram().c_str());
    clearColumn(window, row, columns, "time_column");
    mvwprintw(window, row, columns["time_column"],
              "%s", Format::ElapsedTime(processes[i].UpTime()).c_str());
    clearColumn(window, row, columns, "command_column");
    mvwprintw(window, row, columns["command_column"],
              "%s", processes[i].Command().substr(0, window->_maxx - 46).c_str());
  }
}

void NCursesDisplay::Display(System& system, int n) {
  initscr();      // start ncurses
  noecho();       // do not print input values
  cbreak();       // terminate ncurses on ctrl + c
  start_color();  // enable color

  int x_max{getmaxx(stdscr)};
  WINDOW* system_window = newwin(9, x_max - 1, 0, 0);
  WINDOW* process_window =
      newwin(3 + n, x_max - 1, system_window->_maxy + 1, 0);

  while (1) {
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    box(system_window, 0, 0);
    box(process_window, 0, 0);
    DisplaySystem(system, system_window);
    DisplayProcesses(system.Processes(), process_window, n);
    wrefresh(system_window);
    wrefresh(process_window);
    refresh();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  endwin();
}
