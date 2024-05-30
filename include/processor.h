#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
    float Utilization();

 private:
    long previous_active_jiffies_{0};
    long previous_idle_jiffies_{0};
};

#endif
