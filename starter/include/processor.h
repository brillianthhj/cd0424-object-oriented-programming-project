#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>

class Processor {
 public:
  // cat /proc/stat
  // cpu jiffies user(1st), system(3rd)
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
};

#endif