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

    // Example of how to use DebugOutput to output variables to a debug.txt file
    // LinuxParser::DebugOutput("pids", pids[0]);

    std::vector<int> existing_pids;
    for (Process& process : processes_) {
        existing_pids.emplace_back(process.Pid());
    }

    for (int pid : pids) {
        if (std::find(existing_pids.begin(), existing_pids.end(), pid) == existing_pids.end()) {
            processes_.emplace_back(pid);
        }
    }

    for (auto& process : processes_) {
        process.CpuUtilization(LinuxParser::ActiveJiffies(process.Pid()),
                               LinuxParser::Jiffies());
    }

    // Sort processes by CPU utilization using the overloaded operator< in process.cpp
    std::sort(processes_.rbegin(), processes_.rend());

    return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() const { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() const { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() const { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() const { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() const { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long int System::UpTime() const { return LinuxParser::UpTime(); }
