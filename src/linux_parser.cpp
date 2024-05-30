#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

using std::stof;
using std::string;
using std::to_string;
using std::vector;


/////////////////// Functions for system.cpp //////////////////////

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  std::filesystem::path procDirectory(kProcDirectory);
  for (const auto& entry : std::filesystem::directory_iterator(procDirectory)) {
    if (entry.is_directory()) {
      string filename = entry.path().filename().string();
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  return pids;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string token, mem, line = "";
  float total_mem, free_mem = 0.0f;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> token;
      if (token == kTokenMemTotal) {
        linestream >> total_mem;
      } else if (token == kTokenMemFree) {
        linestream >> free_mem;
        break;
      }
    }
  }
  if (total_mem != 0.0f && free_mem != 0.0f) {
    return (total_mem - free_mem) / total_mem;
  }
  return 0.0f; // Add a default return value in case the conditions are not met
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == kTokenOperatingSystem) {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}
// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string token, processes, line = "";
  int running_processes = 0;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> token;
      if (token == kTokenRunningProcesses) {
        linestream >> running_processes;
        break;
      }
    }
  }
  if (running_processes != 0) {
    return running_processes;
  }

  return 0; // Add a default return value in case the conditions are not met
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string token, processes, line = "";
  int total_processes = 0;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> token;
      if (token == kTokenProcesses) {
        linestream >> total_processes;
        break;
      }
    }
  }
  if (total_processes != 0) {
    return total_processes;
  }
  return 0; // Add a default return value in case the conditions are not met
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string line, uptime, idletime;
  long up_time = 0;

  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
    up_time = std::stol(uptime);
  }
  if (up_time != 0) {
    return up_time;
  }

  return 0; // Add a default return value in case the conditions are not met
}

/////////////////// Functions for process.cpp //////////////////////

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string token, line = "";
  vector<string> cpu_utilization;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> token) {
      if (token != "cpu") {
        cpu_utilization.emplace_back(token);
      }
    }
  }
  return cpu_utilization;
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string token, uid, line = "";

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> token;
      if (token == kTokenUid) {
        linestream >> uid;
        break;
      }
    }
  }
  if (uid != "") {
    return uid;
  }
  return ""; // Add a default return value in case the conditions are not met
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string command, line = "";

  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    command = line;
  }
  if (command != "") {
    return command;
  }

  return ""; // Add a default return value in case the conditions are not met
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string token, ram, line = "";

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> token;
      if (token == kTokenProcMem) {
        linestream >> ram;
        break;
      }
    }
  }
  if (ram != "") {
    // Convert from KB to MB
    ram = std::to_string(std::stoi(ram) / 1024);
    return ram;
  } else {
    return "NaN";
}
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string token, user, line = "";
  string uid = Uid(pid);

  std::ifstream stream(kPasswordPath);
  if (stream.is_open() && uid != "") {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> token >> token;
      if (token == uid) {
        break;
      }
    }
  }

  if (user != "") {
    return user;
  } else {
    return "Unknown";
  }
}

// DONE: Read and return the uptime of a process in seconds
long LinuxParser::UpTime(int pid) {
  string token, line = "";
  vector<string> stat_values;
  long up_time_in_clock_ticks, up_time_in_sec = 0;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> token) {
      stat_values.emplace_back(token);
    }
    up_time_in_clock_ticks = std::stol(stat_values[kTokenUpTimeIndex]);
  }

  if (up_time_in_clock_ticks != 0) {
    up_time_in_sec = up_time_in_clock_ticks / sysconf(_SC_CLK_TCK);
    return up_time_in_sec;
  }
  return 0; // Add a default return value in case the conditions are not met
}


/////// Functions for processor.cpp and helper functions ///////////

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies();}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string token, line = "";
  vector<string> stat_values;
  long active_jiffies = 0;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> token) {
      stat_values.emplace_back(token);
    }
    active_jiffies = std::stol(stat_values[13]) + std::stol(stat_values[14]) + std::stol(stat_values[15]) + std::stol(stat_values[16]);
  }
  if (active_jiffies != 0) {
    return active_jiffies;
  }
  return 0; // Add a default return value in case the conditions are not met
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> cpu_utilization = CpuUtilization();
  long active_jiffies = 0;

  for (int i = kUser_; i <= kGuestNice_; i++) {
    if (i == kIdle_ || i == kIOwait_) {
      continue;
    }
    active_jiffies += std::stol(cpu_utilization[i]);
  }

  if (active_jiffies != 0) {
    return active_jiffies;
  }
  return 0; // Add a default return value in case the conditions are not met
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> cpu_utilization = CpuUtilization();
  long idle_jiffies = 0;

  idle_jiffies = std::stol(cpu_utilization[kIdle_]) + std::stol(cpu_utilization[kIOwait_]);

  if (idle_jiffies != 0) {
    return idle_jiffies;
  }
  return 0; // Add a default return value in case the conditions are not met
}






