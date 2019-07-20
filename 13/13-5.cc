#include <cstdio>
#include <getopt.h>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {
  int num_lines = 10;
  int opt;
  while ((opt = getopt(argc, argv, "n:")) != -1) {
    switch (opt) {
    case 'n':
      num_lines = atoi(optarg);
      break;
    }
  }
  string filename = argv[optind];
  vector<string> lines(num_lines);
  int lines_idx = 0;
  vector<char> buffer(4096);
  size_t num_bytes_read;
  FILE *file = fopen(filename.c_str(), "r");
  while ((num_bytes_read =
              fread(&buffer.front(), sizeof(char), buffer.size(), file)) > 0) {
    for (size_t i = 0; i < num_bytes_read; i++) {
      if (buffer[i] == '\n') {
        lines_idx = (lines_idx + 1) % num_lines;
        lines[lines_idx].clear();
      } else {
        lines[lines_idx] += buffer[i];
      }
    }
  }
  fclose(file);
  for (int i = 1; i <= num_lines; i++) {
    auto &&line = lines[(lines_idx + i) % num_lines];
    line += '\n';
    fwrite(line.c_str(), sizeof(char), line.length(), stdout);
  }
  return 0;
}
