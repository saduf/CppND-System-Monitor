#include "processor.h"
#include <iostream>
#include <unistd.h>

#include "linux_parser.h"

using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
// Reference: https://www.idnt.net/en-US/kb/941772
float Processor::Utilization() {
    long cpuSum = 0.0;
    long cpuLastSum = 0.0;
    long cpuIdle = 0;
    long cpuIdlePast = 0;

    cpuLastSum = LinuxParser::ActiveJiffies();

    cpuIdlePast = LinuxParser::IdleJiffies();


    // Sleep 1 seconds to take cpu delta reading
    sleep(1);

    cpuSum = LinuxParser::ActiveJiffies();

    cpuIdle = LinuxParser::IdleJiffies();

    // Get the delta between two reads
    long cpuDelta = cpuSum - cpuLastSum;

    // Get the idle time Delta 
    long cpuIdleDelta = cpuIdle - cpuIdlePast;

    // Calc time spent working 
    long cpuUsed = cpuDelta - cpuIdleDelta;

    // Calc percentage 
    float cpuUsage = cpuUsed * 1.0f / cpuDelta;

    return cpuUsage; 
}
