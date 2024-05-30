#include "linux_parser.h"
#include "processor.h"

#include <string>
#include <vector>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    float cpu_utilization = 0.0;
    long active_jiffies = LinuxParser::ActiveJiffies();
    long idle_jiffies = LinuxParser::IdleJiffies();

    long duration_active{active_jiffies - PreviousActiveJiffies()};
    long duration_idle{idle_jiffies - PreviousIdleJiffies()};
    long duration{duration_active + duration_idle};

    cpu_utilization = static_cast<float>(duration_active) / duration;

    // Update previous values
    PreviousActiveJiffies(active_jiffies);
    PreviousIdleJiffies(idle_jiffies);

    return cpu_utilization;
}

long Processor::PreviousActiveJiffies() const {
    LinuxParser::DebugOutput("previous_active_jiffies_", previous_active_jiffies_);
    return previous_active_jiffies_;
}

long Processor::PreviousIdleJiffies() const {
    LinuxParser::DebugOutput("previous_idle_jiffies_", previous_idle_jiffies_);
    return previous_idle_jiffies_;
}

long Processor::PreviousActiveJiffies(long active_jiffies) {
    previous_active_jiffies_ = active_jiffies;
    return previous_active_jiffies_;
}

long Processor::PreviousIdleJiffies(long idle_jiffies) {
    previous_idle_jiffies_ = idle_jiffies;
    return previous_idle_jiffies_;
}
