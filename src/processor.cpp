#include "linux_parser.h"
#include "processor.h"

#include <string>
#include <vector>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    float cpu_utilization = 0.0;
    long active_jiffies = LinuxParser::ActiveJiffies();
    long idle_jiffies = LinuxParser::IdleJiffies();

    long duration_active{active_jiffies - previous_active_jiffies_};
    long duration_idle{idle_jiffies - previous_idle_jiffies_};
    long duration{duration_active + duration_idle};

    cpu_utilization = static_cast<float>(duration_active) / duration;

    // Update previous values
    previous_active_jiffies_ = active_jiffies;
    previous_idle_jiffies_ = idle_jiffies;

    return cpu_utilization;
}
