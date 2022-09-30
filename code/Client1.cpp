//Client1 (socket)

#include <iostream>
#include <winsock2.h>
//#pragma comment (lib, "Ws2_32.lib")

using namespace std;

float calculate_mean(char buffer[], int n);

int main()
{
    printf("Client1 is ready\n");

    //initialize
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
    {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        getchar();
        return 1;
    }

    //create socket
    SOCKET client_socket;
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET)
    {
        printf("Socket failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        getchar();
        return 1;
    }

    char ip[]="127.0.0.1";
    int port=27015;
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_port = htons(port);

	//Connect to server
	while(connect(client_socket, (struct sockaddr*)&server, sizeof(server)) < 0)
	{

	}

    const int BUFSIZE=1024;
    char buffer[BUFSIZE];
    //receive from socket
    int bytesRead = recv(client_socket, buffer, BUFSIZE, 0);
    if(bytesRead == SOCKET_ERROR)
    {
        printf("recv failed with error : %d" , WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        getchar();
        return 1;
    }
    buffer[bytesRead] = '\0';

    float output=calculate_mean(buffer, bytesRead);
    printf("mean : %f \n", output);

    snprintf(buffer, sizeof(buffer), "%f", output);
    //send to socket
    if(send(client_socket, buffer, strlen(buffer), 0) == SOCKET_ERROR)
    {
        printf("send failed with error : %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        getchar();
        return 1;
    }

    closesocket(client_socket);
    WSACleanup();
    getchar();
    return 0;
}

float calculate_mean(char buffer[], int n)
{
    int mean=0;
    int cnt=0;
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
            sign=1;
            ++i;
        }

        int num=0;
        while(i < n && isdigit(buffer[i]))
        {
            num=num*10+(buffer[i]-'0');
            ++i;
        }

        mean+=sign*num;
        ++cnt;
    }

    //cout<<mean<<" "<<cnt<<endl;
    if(cnt==0) return 0;
    return mean/(float)cnt;
}
