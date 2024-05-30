#ifndef PROCESS_H
#define PROCESS_H

#include <string>
using std::string;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
public:
   Process(int pid);
   int Pid() const;
   string User();
   string Command();
   float CpuUtilization();
   void CpuUtilization(long process_jiffies, long total_jiffies);
   string Ram();
   long int UpTime();
   bool operator<(Process const& a) const;

private:
   int pid_ = 0;
   string user_ = "";
   string ram_ = "";
   string command_ = "";
   float process_cpu_utilization_{0.0};
   long int up_time_{0};
   long previous_process_jiffies_{0};
   long previous_total_jiffies_{0};
};

#endif
