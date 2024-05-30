#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

using std::string;
using std::vector;

namespace LinuxParser {
// Paths
const string kProcDirectory{"/proc/"};
const string kCmdlineFilename{"/cmdline"};
const string kCpuinfoFilename{"/cpuinfo"};
const string kStatusFilename{"/status"};
const string kStatFilename{"/stat"};
const string kUptimeFilename{"/uptime"};
const string kMeminfoFilename{"/meminfo"};
const string kVersionFilename{"/version"};
const string kOSPath{"/etc/os-release"};
const string kPasswordPath{"/etc/passwd"};

// Tokens
const string kTokenProcesses{"processes"};
const string kTokenRunningProcesses{"procs_running"};
const string kTokenMemTotal{"MemTotal:"};
const string kTokenMemFree{"MemFree:"};
const string kTokenOperatingSystem{"PRETTY_NAME"};
const string kTokenUid{"Uid:"};
const string kTokenProcMem{"VmRSS:"};
const int kTokenUpTimeIndex{21};

// System
float MemoryUtilization();
long UpTime();
vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
string OperatingSystem();
string Kernel();

// Utils
// Template for outputting variables to a debug.txt file because the terminal is used to display the system monitor
template <typename T>
void DebugOutput(const std::string& variableName, const T& variable) {
  std::ofstream debugFile("debug.txt", std::ios::app);
  if (debugFile.is_open()) {
    debugFile << variableName << ": " << variable << std::endl;
    debugFile.close();
  }
}

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
vector<string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
string Command(int pid);
string Ram(int pid);
string Uid(int pid);
string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif
