#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
    long previous_active_jiffies_{0};
    long previous_idle_jiffies_{0};
};

#endif
