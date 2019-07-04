#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int opt;
  bool append = false;
  while ((opt = getopt(argc, argv, "a")) != -1) {
    switch (opt) {
    case 'a':
      append = true;
      break;
    }
  }
  char *file = argv[optind];
  int append_or_truncate;
  if (append) {
    append_or_truncate = O_APPEND;
  } else {
    append_or_truncate = O_TRUNC;
  }
  int fd =
      open(file, O_WRONLY | O_CREAT | append_or_truncate, S_IRUSR | S_IWUSR);
  const int buffer_size = 300;
  void *buffer[buffer_size];
  int num_bytes_read;
  while ((num_bytes_read = read(STDIN_FILENO, buffer, buffer_size)) != -1) {
    write(STDOUT_FILENO, buffer, num_bytes_read);
    write(fd, buffer, num_bytes_read);
  }
  close(fd);
  return 0;
}
