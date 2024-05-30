#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>
#include <sstream>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using std::stof;
using std::string;
using std::to_string;
using std::vector;


// Template for outputting variables to a debug.txt file because the terminal is used to display the system monitor
template <typename T>
void DebugOutput(const std::string& variableName, const T& variable) {
  std::ofstream debugFile("debug.txt", std::ios::app);
  if (debugFile.is_open()) {
    debugFile << variableName << ": " << variable << std::endl;
    debugFile.close();
  }
}

/////////////////// Functions for system.cpp //////////////////////

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
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

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) {
  string command, line = "";

  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    command = line;
  }
  if (command != "") {
    return command;
  }
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }



/////// Functions for processor.cpp and helper functions ///////////

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies();}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
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
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> cpu_utilization = CpuUtilization();
  long idle_jiffies = 0;

  idle_jiffies = std::stol(cpu_utilization[kIdle_]) + std::stol(cpu_utilization[kIOwait_]);

  if (idle_jiffies != 0) {
    return idle_jiffies;
  }
}

// DONE: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) {
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
}




