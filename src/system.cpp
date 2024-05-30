#include "system.h"

#include <unistd.h>
#include "linux_parser.h"

#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
    vector<int> pids = LinuxParser::Pids();

    LinuxParser::DebugOutput("pids", pids[0]);

    set<int> existing_pids;
    for (Process& process : processes_) {
        existing_pids.insert(process.Pid());
    }

    for (int pid : pids) {
        if (existing_pids.find(pid) == existing_pids.end()) {
            processes_.emplace_back(pid);
        }
    }

    for (auto& process : processes_) {
        process.CpuUtilization(LinuxParser::ActiveJiffies(process.Pid()),
                               LinuxParser::Jiffies());
    }
    std::sort(processes_.rbegin(), processes_.rend());

    return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
