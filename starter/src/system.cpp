#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric
criteria "The student will be able to extract and display basic data about the
system."

You need to properly format the uptime. Refer to the comments mentioned in
format. cpp for formatting the uptime.*/

System::System() {
  kernel_ = LinuxParser::Kernel();
  os_ = LinuxParser::OperatingSystem();
}
// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_.clear();
  std::vector pids = LinuxParser::Pids();
  for (const auto pid : pids) {
    string user = LinuxParser::User(pid);
    string command = LinuxParser::Command(pid);
    float cpu = LinuxParser::CpuUtilization(pid);
    string ram = LinuxParser::Ram(pid);
    long int time = LinuxParser::UpTime(pid);

    Process proc(pid, user, command, cpu, ram, time);
    processes_.push_back(proc);
  }
  std::sort(processes_.begin(), processes_.end());
  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return kernel_; }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return os_; }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
