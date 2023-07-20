#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	FILE *fptr;
	char c;
	unsigned long counter = 0;
	
	fptr = fopen("loremipsum.txt", "r");
	
	if (fptr == NULL)
        exit(EXIT_FAILURE);
		
	time_t t, start, end;
	start = clock();
    
    while((c = fgetc(fptr)) != EOF)
    {
		if(c == 'a'){counter++;}
    }
	
	fclose(fptr);

	end = clock();
	t = (end - start)/CLOCKS_PER_SEC;
	
	printf("Number of chars is %d.\n", counter);
	printf("Found in %d seconds.\n", t);
	
	return 0;
}