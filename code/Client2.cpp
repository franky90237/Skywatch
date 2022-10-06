//Client2 (pipe)

#include <windows.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

float calculate_median(char buffer[], int n);

int main(void)
{
    printf("Client2 is ready\n");

	HANDLE pipe;
	const char pipeName[]="\\\\.\\Pipe\\myNamedPipe";

    //wait for pipe
	while(WaitNamedPipe(pipeName, NMPWAIT_WAIT_FOREVER) == 0)
	{
		
	}

    //connect to pipe
	if((pipe = CreateFile(pipeName, GENERIC_READ | GENERIC_WRITE,0 , (LPSECURITY_ATTRIBUTES)NULL,
                          OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL)) == INVALID_HANDLE_VALUE)
	{
		printf("CreateFile failed with error : %ld \n", GetLastError());
		getchar();
		return 1;
	}

    const int BUFSIZE=1024;
    char buffer[BUFSIZE];
    DWORD bytesRead;
    //read from pipe
    if(ReadFile(pipe, buffer, sizeof(buffer), &bytesRead, NULL) <= 0)
	{
		printf("ReadFile failed with error : %ld \n",GetLastError());
		CloseHandle(pipe);
		getchar();
		return 1;
	}
	buffer[bytesRead]='\0';

	//printf("buffer = %s, %ld characters\n", buffer, bytesRead);
	float output=calculate_median(buffer, bytesRead);
    printf("median : %f \n", output);
    snprintf(buffer, sizeof(buffer), "%f", output);

    DWORD bytesWrite;
    //write to pipe
	if(WriteFile(pipe, buffer, strlen(buffer), &bytesWrite, NULL) == 0)
	{
		printf("WriteFile failed with error : %ld \n",GetLastError());
		CloseHandle(pipe);
		getchar();
		return 1;
	}

	CloseHandle(pipe);
	getchar();
	return 0;
}

float calculate_median(char buffer[], int n)
{
    vector<int> nums;
    int i=0;
    while(i < n)
    {
        if(buffer[i] == ' ')
        {
            ++i;
            continue;
        }

        int sign=1;
        if(i < n && buffer[i] == '-')
        {
            sign=-1;
            ++i;
        }
        else if(i < n && buffer[i] == '+')
        {
            sign=+1;
            ++i;
        }

        int num=0;
        while(i < n && isdigit(buffer[i]))
        {
            num=num*10+(buffer[i]-'0');
            ++i;
        }

        nums.push_back(sign*num);
    }

    sort(nums.begin(), nums.end());
    //for(auto& num : nums) cout<<num<<" ";
    //cout<<endl;

    n=nums.size();
    if(n==0) return 0;
    return (nums[(n-1)/2] + nums[n/2]) / 2.0;
}
