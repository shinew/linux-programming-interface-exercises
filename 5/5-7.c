#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>

size_t min(size_t a, size_t b) {
  if (a < b) {
    return a;
  } else {
    return b;
  }
}

ssize_t get_total_size(const struct iovec *iov, int iovcnt) {
  ssize_t total_size = 0;
  for (int i = 0; i < iovcnt; i++) {
    total_size += iov[i].iov_len;
  }
  return total_size;
}

ssize_t readv(int fd, const struct iovec *iov, int iovcnt) {
  const ssize_t total_size = get_total_size(iov, iovcnt);
  void *buffer = malloc(total_size);
  const size_t num_bytes_read = read(fd, buffer, total_size);
  ssize_t offset = 0;
  for (int i = 0; i < iovcnt; i++) {
    const ssize_t bytes_for_this_buffer =
        min(num_bytes_read - offset, iov[i].iov_len);
    memcpy(iov[i].iov_base, &buffer[offset], bytes_for_this_buffer);
    offset += bytes_for_this_buffer;
  }
  free(buffer);
  return num_bytes_read;
}

ssize_t writev(int fd, const struct iovec *iov, int iovcnt) {
  ssize_t total_size = get_total_size(iov, iovcnt);
  void *buffer = malloc(total_size);
  ssize_t offset = 0;
  for (int i = 0; i < iovcnt; i++) {
    memcpy(&buffer[offset], iov[i].iov_base, iov[i].iov_len);
    offset += iov[i].iov_len;
  }
  ssize_t num_bytes_written = write(fd, buffer, total_size);
  free(buffer);
  return num_bytes_written;
}

int main() {
  char buf1[10];
  char buf2[10];
  struct iovec iov[2];
  iov[0].iov_base = buf1;
  iov[0].iov_len = sizeof(buf1);
  iov[1].iov_base = buf2;
  iov[1].iov_len = sizeof(buf2);
  ssize_t read = readv(STDIN_FILENO, iov, 2);
  printf("read: %ld\n", read);
  ssize_t written = writev(STDOUT_FILENO, iov, 2);
  printf("wrote: %ld\n", written);
  return 0;
}
