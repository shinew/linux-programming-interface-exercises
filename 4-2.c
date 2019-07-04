#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

int main(int __attribute__((unused)) argc, const char *argv[]) {
  const int from = open(argv[1], O_RDONLY);
  const int to = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

  const int buffer_size = 4096;
  char buffer[buffer_size];
  off_t from_base_offset = 0;
  off_t to_end_offset = -1;

  int num_bytes_read;
  while ((num_bytes_read = read(from, buffer, buffer_size)) > 0) {
    int start_of_non_null_segment = -1;
    for (int i = 0; i < num_bytes_read; i++) {
      if (buffer[i] == '\0') {
        if (start_of_non_null_segment > -1) {
          // buffer[start_of_non_null_segment : i] are non-null
          lseek(to, from_base_offset + start_of_non_null_segment, SEEK_SET);
          write(to, buffer + start_of_non_null_segment,
                i - start_of_non_null_segment);
          to_end_offset = from_base_offset + i;
          start_of_non_null_segment = -1;
        }
      } else {
        if (start_of_non_null_segment == -1) {
          start_of_non_null_segment = i;
        }
      }
    }
    if (start_of_non_null_segment > -1) {
      // buffer[start_of_non_null_segment : num_bytes_read] are non-null
      lseek(to, from_base_offset + start_of_non_null_segment, SEEK_SET);
      write(to, buffer + start_of_non_null_segment,
            num_bytes_read - start_of_non_null_segment);
      to_end_offset = from_base_offset + num_bytes_read;
    }
    from_base_offset += num_bytes_read;
  }
  // edge case: a bunch of terminating nulls, so we need to ensure file sizes
  // are equal
  if (to_end_offset < from_base_offset) {
    buffer[0] = '\0';
    lseek(to, from_base_offset - 1, SEEK_SET);
    write(to, buffer, 1);
  }
  close(from);
  close(to);
  return 0;
}
