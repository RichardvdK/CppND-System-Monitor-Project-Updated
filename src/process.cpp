#include "process.h"
#include "linux_parser.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid){
  pid_ = pid;
  ram_ = LinuxParser::Ram(pid_);
  user_ = LinuxParser::User(pid_);
  command_ = LinuxParser::Command(pid_);
  up_time_ = LinuxParser::UpTime(pid_);
}

// DONE: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return process_cpu_utilization_; }

void Process::CpuUtilization(long process_jiffies, long total_jiffies) {
  long duration_process{process_jiffies - previous_process_jiffies_};
  long duration_total{total_jiffies - previous_total_jiffies_};
  long duration{duration_process + duration_total};

  process_cpu_utilization_ = static_cast<float>(duration_process) / duration;

  // Update previous values
  previous_process_jiffies_ = process_jiffies;
  previous_total_jiffies_ = total_jiffies;
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return up_time_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return process_cpu_utilization_ < a.process_cpu_utilization_;
}
