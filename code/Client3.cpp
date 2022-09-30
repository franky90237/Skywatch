//Client3 (shared memory)

#include <windows.h>
#include <iostream>
#include <map>

using namespace std;

char* print_and_calculateMode(char input[], int n);

int main()
{
    printf("Client3 is ready \n");

    const int BUFSIZE=1024;
    char mappedName[]="mySharedMemoey";
    HANDLE sharedMemory;

    //wait for shared memory
    do
    {
        sharedMemory = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, mappedName);
    } while(sharedMemory == NULL);

    //get the address of shared memory
    LPCTSTR buffer = (LPTSTR) MapViewOfFile(sharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, BUFSIZE);
    if (buffer == NULL)
    {
        printf("MapViewOfFile failed with error : %ld \n", GetLastError());
        CloseHandle(sharedMemory);
        getchar();
        return 1;
    }

    //read from shared memory
    char input[strlen(buffer)];
    strcpy(input, buffer);
    char* output = print_and_calculateMode(input, strlen(input));
    printf("mode : %s \n", output+1);

    //write to shared memory
    CopyMemory((void*)buffer, output, strlen(output)+1);

    UnmapViewOfFile(buffer);
    CloseHandle(sharedMemory);

    delete [] output;
    getchar();
    return 0;
}

char* print_and_calculateMode(char input[], int n)
{
    map<int, int> nums;
    int maxCnt=0;
    int i=0;
    while(i < n)
    {
        if(input[i] == ' ')
        {
            ++i;
            continue;
        }

        int sign=1;
        if(i < n && input[i] == '-')
        {
            sign=-1;
            ++i;
        }
        else if(i < n && input[i] == '+')
        {
            sign=1;
            ++i;
        }

        int num=0;
        while(i < n && isdigit(input[i]))
        {
            num=num*10+(input[i]-'0');
            ++i;
        }

        ++nums[sign*num];
        maxCnt=max(maxCnt, nums[sign*num]);
    }

    //A represents acknowledgement
    string tmp="A";
    for(map<int, int>::iterator it=nums.begin(); it!=nums.end(); ++it)
    {
        if(it->second == maxCnt)
        {
            //printf("%d ", it->first);
            tmp= tmp + to_string(it->first) + " ";
        }
    }

    if(!tmp.empty()) tmp.pop_back();
    char* output=new char[tmp.size()+1];
    strcpy(output, tmp.c_str());
    output[tmp.size()]='\0';

    return output;
}
