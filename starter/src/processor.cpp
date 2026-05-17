#include "processor.h"

#include "linux_parser.h"

using std::stol;
using std::string;
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  float percent = 0.f;
  string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  string line;
  std::ifstream stream(LinuxParser::kProcDirectory +
                       LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >>
        softirq >> steal >> guest >> guest_nice;

    long usertime = stol(user) - stol(guest);
    long nicetime = stol(nice) - stol(guest_nice);
    unsigned long long int idletime = stol(idle) + stol(iowait);
    unsigned long long int systemalltime =
        stol(system) + stol(irq) + stol(softirq);
    unsigned long long int virtalltime = stol(guest) + stol(guest_nice);
    unsigned long long int totaltime = usertime + nicetime + systemalltime +
                                       idletime + stol(steal) + virtalltime;

    percent = static_cast<float>(totaltime - idletime) / totaltime;
  }

  return percent;
}