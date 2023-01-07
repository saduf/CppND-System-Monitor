#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  std::string result;
  //convret seconds into hr, min, sec
  string hr = std::to_string((int)(seconds/3600));
  string min = std::to_string(((int)(seconds/60))%60);
  string sec = std::to_string((int)(seconds%60));

  result = hr + ":" + min + ":" + sec;

  return result;
}