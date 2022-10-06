#include <windows.h>
#include <iostream>
#include <winsock2.h>
//#pragma comment (lib, "Ws2_32.lib")

using namespace std;

string communicate_via_socket(char input[]);
string communicate_via_pipe(char input[]);
string communicate_via_sharedMemery(char input[]);

int main()
{
    cout<<"Server is ready. You can type intergers and then click [ENTER]. \n";
    cout<<"Clients will show the mean, median, and mode of the input values.\n";

    const int MAX_INPUT_SIZE=1024;
    char input[MAX_INPUT_SIZE];
    cin.getline(input, MAX_INPUT_SIZE);

    string mean=communicate_via_socket(input);
    string median=communicate_via_pipe(input);
    string mode=communicate_via_sharedMemery(input);

    cout<<"mean   :  "<<mean<<endl;
    cout<<"median :  "<<median<<endl;
    cout<<"mode   :  "<<mode<<endl;

    getchar();
    return 0;
}

string communicate_via_socket(char input[])
{
    printf("Connecting to Client1 \n");

    //initialize
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
    {
        printf("WSAStartup failed with error : %d\n", WSAGetLastError());
        return "";
    }

    //create socket
    SOCKET listen_socket;
    listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_socket == INVALID_SOCKET)
    {
        printf("Socket failed with error : %d\n", WSAGetLastError());
        WSACleanup();
        return "";
    }

    sockaddr_in service;
    char ip[]="127.0.0.1";
    int port=27015;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr(ip);
    service.sin_port = htons(port);

    //bind socket to IP and port
    if (bind(listen_socket, (SOCKADDR *) & service, sizeof (service)) == SOCKET_ERROR)
    {
        printf("Bind failed with error : %d\n", WSAGetLastError());
        closesocket(listen_socket);
        WSACleanup();
        return "";
    }
    //printf("Binding to %s : %d\n", ip, port);

    //set socket to listen state
    if (listen(listen_socket, 1) == SOCKET_ERROR)
    {
        printf("Listen failed with error : %d\n", WSAGetLastError());
        closesocket(listen_socket);
        WSACleanup();
        return "";
    }

    SOCKET client_socket = accept(listen_socket, NULL, NULL);
    if(client_socket == INVALID_SOCKET)
    {
        printf("accept failed with error : %d\n", WSAGetLastError());
        closesocket(listen_socket);
        WSACleanup();
        return "";
    }

    //send to client socket
    if(send(client_socket, input, strlen(input), 0) == SOCKET_ERROR)
    {
        printf("send failed with error : %d\n", WSAGetLastError());
        closesocket(listen_socket);
        WSACleanup();
        return "";
    }

    //receive from client socket
    const int BUFSIZE=1024;
    char buffer[BUFSIZE];
    int bytesRead = recv(client_socket, buffer, BUFSIZE, 0);
    if(bytesRead == SOCKET_ERROR)
    {
        printf("recv failed with error : %d" , WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return"";
    }
    buffer[bytesRead] = '\0';
    string output(buffer);

    closesocket(listen_socket);
    WSACleanup();

    return output;
}

string communicate_via_pipe(char input[])
{
    printf("Connecting to Client2 \n");

    HANDLE pipe;

    //create pipe
    if((pipe = CreateNamedPipe("\\\\.\\Pipe\\myNamedPipe", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE,
                                 1, 0, 0, NMPWAIT_USE_DEFAULT_WAIT, NULL)) == INVALID_HANDLE_VALUE)
    {
        printf("CreateNamedPipe failed with error : %ld \n",GetLastError());
        return "";
    }

    //connect to pipe
    if (ConnectNamedPipe(pipe, NULL) == 0)
    {
        printf("ConnectNamePipe failed with error : %ld \n",GetLastError());
        CloseHandle(pipe);
        return "";
    }
    
    DWORD bytesWrite;
    //write to pipe
    if(WriteFile(pipe, input, strlen(input), &bytesWrite, NULL) == 0)
    {
        printf("WriteFile failed with error : %ld \n",GetLastError());
        CloseHandle(pipe);
        return "";
    }

    const int BUFSIZE=1024;
    char buffer[BUFSIZE];
    DWORD bytesRead;
    //read form pipe
    if(ReadFile(pipe, buffer, sizeof(buffer), &bytesRead, NULL) <= 0)
    {
        printf("ReadFile failed with error : %ld \n",GetLastError());
        CloseHandle(pipe);
        return "";
    }
    buffer[bytesRead]='\0';
    string output(buffer);

    if (DisconnectNamedPipe(pipe) == 0)
    {
        printf("DisconnectNamedPipe failed with error:  %ld \n",GetLastError());
        return "";
    }

    CloseHandle(pipe);

    return output;
}

string communicate_via_sharedMemery(char input[])
{
    printf("Connecting to Client3 \n");

    const int BUFSIZE=1024;
    char mappedName[]="mySharedMemoey";
    HANDLE sharedMemory;

    //create shared memory
    sharedMemory = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUFSIZE, mappedName);
    if(sharedMemory == NULL)
    {
        printf("CreateFileMapping failed with error : %ld \n", GetLastError());
        return "";
    }

    //get address of shared memory
    LPCTSTR buffer = (LPTSTR) MapViewOfFile(sharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, BUFSIZE);
    if (buffer == NULL)
    {
        printf("MapViewOfFile failed with error : %ld \n", GetLastError());
        CloseHandle(sharedMemory);
        return "";
    }

    //write to shared memory
    CopyMemory((void*)buffer, input, strlen(input)+1);

    //read from share memory
    while(buffer[0] != 'A')
    {

    }

    string output;
    for(int i=1; buffer[i]!='\0'; ++i)
    {
        output.push_back(buffer[i]);
    }

    UnmapViewOfFile(buffer);
    CloseHandle(sharedMemory);

    return output;
}
