#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process() {}
  Process(int pid, std::string user, float cpu, std::string ram, long int time,
          std::string command)
      : pid_(pid),
        user_(user),
        utilization_(cpu),
        ram_(ram),
        uptime_(time),
        command_(command) {}

  int Pid() const;                         // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization() const;            // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid_;
  std::string user_;
  float utilization_;
  std::string ram_;
  long int uptime_;
  std::string command_;
};

#endif