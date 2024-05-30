#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
   float Utilization();
   long PreviousActiveJiffies() const;
   long PreviousIdleJiffies() const;
   long PreviousActiveJiffies(long active_jiffies);
   long PreviousIdleJiffies(long idle_jiffies);

 private:
    long previous_active_jiffies_{0};
    long previous_idle_jiffies_{0};
};

#endif
