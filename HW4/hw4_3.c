#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main()
{
	int fd = open("loremipsum.txt", O_RDONLY);
	struct stat s;	
	int status = fstat(fd, &s);
	size_t size;
	size = s.st_size;
	
	// Map the file
	char *ptr = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(ptr == MAP_FAILED)
	{
		printf("Mapping failed!\n");
		return 1;
	}
		
	close(fd);
	
	time_t t, start, end;
	start = clock();
	
	// Read file through ptr
	unsigned long counter = 0;
	size_t i = 0;
	for( ; i < size; i++)
	{
		if(ptr[i] == 'a'){counter++;}
	}	
	
	end = clock();
	t = (end - start)/CLOCKS_PER_SEC;
	
	printf("Number of chars is %i.\n", counter);
	printf("Found in %d seconds.\n", t);

	// Unmap the file
	status = munmap(ptr, size);
	if(status != 0)
	{
		printf("Unmapping failed!\n");
		return 1;
	}
	
	return 0;
}