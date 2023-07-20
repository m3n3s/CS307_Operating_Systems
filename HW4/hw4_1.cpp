#include <iostream>
#include <string>
#include <fstream>
#include <time.h>

using namespace std;

int main()
{
	string line;
	ifstream file("loremipsum.txt");	
	unsigned long counter = 0;
	
	time_t start, end, duration;
	start = clock();
	
	while(getline(file, line))
	{
		for(int i = 0; i < line.length(); i++)
		{
			if(line[i] == 'a'){counter++;}
		}
	}
	file.close();
	
	end = clock();
	duration = (end - start)/CLOCKS_PER_SEC;
	
	cout << "Number of chars is " << counter << endl;
	cout << "Found in " << duration << " seconds." << endl;
	
	return 0;
}