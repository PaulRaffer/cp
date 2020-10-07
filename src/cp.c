#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define ERROR -1


bool is_error(int code);

ssize_t copy(int src_fd, int dest_fd, size_t bytes);
ssize_t copy_file(int src_fd, int dest_fd, size_t bytes_per_iteration);
ssize_t cp(char const * src_path, char const * dest_path);



bool is_error(int code)
{
	return code < 0;
}


ssize_t copy(int src_fd, int dest_fd, size_t bytes)
{
	void * buf[bytes];

	ssize_t const read_num_bytes = read(src_fd, buf, bytes);
	if (is_error(read_num_bytes)) {
		return ERROR;
	}

	return write(dest_fd, buf, read_num_bytes);
}

ssize_t copy_file(int src_fd, int dest_fd, size_t bytes_per_iteration)
{
	ssize_t num_bytes = 0;
	{
	ssize_t copy_num_bytes;
	do {
		copy_num_bytes = copy(src_fd, dest_fd, bytes_per_iteration);
		if (is_error(copy_num_bytes)) {
			return ERROR;
		}
		num_bytes += copy_num_bytes;
	} while (copy_num_bytes == bytes_per_iteration);
	}
	return num_bytes;
}

ssize_t cp(char const * src_path, char const * dest_path)
{
	int const src_fd = open(src_path, O_RDONLY);
	if (is_error(src_fd)) {
		return ERROR;
	}
	int const dest_fd = open(dest_path, O_WRONLY | O_CREAT);
	if (is_error(dest_fd)) {
		close(src_fd);
		return ERROR;
	}
	
	ssize_t num_bytes = copy_file(src_fd, dest_fd, 0xF);
	
	if (is_error(close(dest_fd))) {
		num_bytes = ERROR;
	}
	if (is_error(close(src_fd))) {
		num_bytes = ERROR;
	}

	return num_bytes;
}
	
int main(int argc, char * argv[])
{
	return argc <= 2 || is_error(cp(argv[1], argv[2])) ?
		EXIT_FAILURE :
		EXIT_SUCCESS;
}

