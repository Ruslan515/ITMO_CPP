#include "huffman.h"
#include <map>

const int V = 0;
const int C = 1;
const int D = 2;

int main(int argc, char** argv) {
  std::string file_in, file_out;

  std::map<std::string , int> mapping;
  mapping["-v"] = V;
  mapping["-c"] = C;
  mapping["-d"] = D;
  int com = 0;
  int idx = 0;
  if (argc == 4) {
    int command = mapping[argv[1]];
    switch (command) {
    case C:
      com = 1;
      break;
    case D:
      com = 2;
      break ;
    }
  }
  else if (argc == 5) {
    int command = mapping[argv[2]];
    switch (command) {
    case C:
      com = 3;
      break;
    case D:
      com = 4;
      break ;
    }
    idx++;
  }

  file_in = argv[2 + idx];
  file_out = argv[3 + idx];

  mse::huffman(com, file_in, file_out);

  return 0;
}
