#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() {
    return Process::pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    float hertz = sysconf(_SC_CLK_TCK) * 100.0;
    float activeJiffies = LinuxParser::ActiveJiffies(Process::Pid()) * 1.0;
    float cpuUtilization = activeJiffies / hertz;
    return cpuUtilization;

}

// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(Process::Pid());
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(Process::Pid()); 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(Process::Pid());
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(Process::Pid());
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    
    
    return cpu_utilization_ > a.cpu_utilization_;

}