#include "processor.h"

#include "linux_parser.h"

using std::string;
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<long> contents = LinuxParser::CpuUtilization();
  long idle = contents[0];
  long total = contents[1];

  long idle_delta = idle - prev_idle_;
  long total_delta = total - prev_total_;

  prev_idle_ = idle;
  prev_total_ = total;

  if (total_delta == 0) return 0.0;

  float percent = static_cast<float>(total_delta - idle_delta) / (total_delta);
  return percent;
}