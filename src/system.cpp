#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric
criteria "The student will be able to extract and display basic data about the
system."

You need to properly format the uptime. Refer to the comments mentioned in
format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { 
    return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 

    processes_.clear();
    vector<int> pids = LinuxParser::Pids();
    for (int pid : pids) {
        Process p(pid);
        processes_.push_back(p);
    }
    std::sort(processes_.begin(), processes_.end());
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    kernelID_ = LinuxParser::Kernel();
    return kernelID_; 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
    memoryUtilization_ = LinuxParser::MemoryUtilization();
    return memoryUtilization_; 
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    operatingSystem_ = LinuxParser::OperatingSystem();
    return operatingSystem_; 
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    runningProcesses_ = LinuxParser::RunningProcesses();
    return runningProcesses_; 
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
    totalProcesses_ = LinuxParser::TotalProcesses();
    return totalProcesses_; 
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
    upTime_ = LinuxParser::UpTime();
    return upTime_; 
}
