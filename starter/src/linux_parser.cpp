#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line, key, value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string MemTotal, MemFree, Cached, SReclaimable, Shmem;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      string key, value;
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          MemTotal = value;
        } else if (key == "MemFree") {
          MemFree = value;
        } else if (key == "Cached") {
          Cached = value;
        } else if (key == "SReclaimable") {
          SReclaimable = value;
        } else if (key == "Shmem") {
          Shmem = value;
        }
      }
    }
    float total = stof(MemTotal);
    float free = stof(MemFree);
    float cached = stof(Cached);
    float srecl = stof(SReclaimable);
    float shmem = stof(Shmem);

    return (total - free - cached - srecl + shmem) / total;
  }
  return 0.f;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return stol(uptime);
  }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      string key, value;
      while (linestream >> key >> value) {
        if (key == "processes") return stoi(value);
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      string key, value;
      while (linestream >> key >> value) {
        if (key == "procs_running") return stoi(value);
      }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      string key, value;
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          float mbSize = stof(value) * 0.001;
          std::ostringstream oss;
          oss << std::fixed << std::setprecision(0) << mbSize;
          return oss.str();
        }
      }
    }
  }
  return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      string key, value;
      while (linestream >> key >> value) {
        if (key == "Uid") return value;
      }
    }
  }
  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid);

  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      string user, x, id1, id2;
      while (linestream >> user >> x >> id1 >> id2) {
        if (id1 == uid && id2 == uid) return user;
      }
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  long clockTic = sysconf(_SC_CLK_TCK);
  int startTimeOrder = 22;
  std::vector<string> contents(startTimeOrder, "");

  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < static_cast<int>(contents.size()); ++i) {
      linestream >> contents[i];
    }
    long starttime = stol(contents[startTimeOrder - 1]);
    long uptime = UpTime();
    return uptime - (starttime / clockTic);
  }

  return 0;
}

float LinuxParser::CpuUtilization(int pid) {
  long clockTic = sysconf(_SC_CLK_TCK);
  int startTimeOrder = 22;
  std::vector<string> contents(startTimeOrder, "");

  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < static_cast<int>(contents.size()); ++i) {
      linestream >> contents[i];
    }
    long utime = stol(contents[13]);
    long stime = stol(contents[14]);
    long cutime = stol(contents[15]);
    long cstime = stol(contents[16]);

    long totaltime = (utime + stime + cutime + cstime) / clockTic;
    long seconds = UpTime(pid);
    return static_cast<float>(totaltime * 1.f / seconds);
  }

  return 0.f;
}
