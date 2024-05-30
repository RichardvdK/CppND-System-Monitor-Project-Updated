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

// Not used yet
const string kTokenCpu{"cpu"};
const string kTokenUser{"User:"};
const string kTokenProcMem{"VmData:"};
const string kTokenStartTime{"btime"};
const string kTokenUpTime{"Uptime:"};




// System
float MemoryUtilization();
long UpTime();
vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
string OperatingSystem();
string Kernel();

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
