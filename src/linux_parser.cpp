#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  float value;
  float memTotal;
  float memFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memTotal = value;
        } else if (key == "MemFree:") {
          memFree = value;
        }
      }
    }
  }
  return (memTotal - memFree) / memTotal; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string key;
  long value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> value;
    }
  }
  return value; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long cpuSum = 0;
  string key;
  float value;
  vector<string> cpuData = LinuxParser::CpuUtilization();
  std::istringstream linestream(cpuData[0]);
  // ignore cpu key
  linestream >> key;
  while (linestream >> value) {
      cpuSum += value;
  }
  return cpuSum; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 

  string line;
  string value;
  long activeJiffies = 0;
  int counter = 1;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) 
                          + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    // std::cout << line << "\n";
    std::istringstream linestream(line);
    while (linestream >> value) {
      if (counter >= 14 && counter <= 17) {
        activeJiffies += stoi(value);
      }
      counter ++;
    }
  }

  return activeJiffies; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 

  long cpuSum = 0;
  string key;
  float value;
  vector<string> cpuData = LinuxParser::CpuUtilization();
  std::istringstream linestream(cpuData[0]);
  // ignore cpu key
  linestream >> key;
  while (linestream >> value) {
      cpuSum += value;
  }

  return cpuSum;   
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string key;
  float value;
  int idleIdx = 1;
  double currentIdle = 0;
  vector<string> cpuData = LinuxParser::CpuUtilization();
  std::istringstream linestream(cpuData[0]);
  // ignore cpu key
  linestream >> key;
  // std::cout << "cpu_line: " << cpuData[0] << "\n";
  while (linestream >> value) {
      if (idleIdx == 4 || idleIdx == 5){
          currentIdle += value;
      }
      idleIdx ++;
  }

  return currentIdle; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string key;
  vector<string> cpuData;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key;
    if (key == "cpu") {
      cpuData.push_back(line);
    }
  }
  return cpuData; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return value; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return value; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid)
                          + kCmdlineFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> value;
  }
  return value; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  long value = 0;
  string ram = std::to_string(0);
  std::ifstream filestream(kProcDirectory + std::to_string(pid) 
                          + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          // Convert to MB
          ram = std::to_string(value / 1000);
          break;
        }
      }
    }
  }
  return ram; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  long value = 0;
  string uid = std::to_string(-1);
  std::ifstream filestream(kProcDirectory  + std::to_string(pid) 
                           + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          // Convert to MB
          uid = std::to_string(value);
          break;
        }
      }
    }
  }
  return uid; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 

  string line;
  string user;
  string value;
  string result = "None";
  long uidMatch = -1;
  std::ifstream filestream(kPasswordPath);
  // Get uid to match with user
  long uid = stoi(LinuxParser::Uid(pid));
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> value >> uidMatch) {
        if ( uid == uidMatch ) {
          result = user;
        }
      }
    }
  }
  return result; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 

  string line;
  string value;
  long clockTicks = 0;
  int counter = 1;
  vector<string> cpuData;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) 
                          + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      if ( counter == 22 ) {
        clockTicks = stoi(value);
      }
      counter++;
    }
  }
  return LinuxParser::UpTime() - (clockTicks/sysconf(_SC_CLK_TCK));
}
