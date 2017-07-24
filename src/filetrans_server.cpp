/*
 * filetrans_server.cpp
 *
 *  Created on: 2017年7月21日
 *      Author: Administrator
 */
/*
#include <WinSock2.h>
#include <stdio.h>
#define PORT 10004
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512
#include <iostream>
using namespace std;
int main(){
	//声明并初始化一个服务端（本地）的地址结构
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	//初始化socket dll
	WSADATA wsaData;
	WORD socketVersion = MAKEWORD(2,0);
	if(WSAStartup(socketVersion,&wsaData)!=0){
		printf("Init socket dll error!");
		exit(1);
	}
	//创建socket
	SOCKET m_Socket = socket(AF_INET,SOCK_STREAM,0);
	if(SOCKET_ERROR ==m_Socket){
		printf("Create Socket Error!");
		exit(1);
	}
	//绑定socket和服务端地址
	if(SOCKET_ERROR ==bind(m_Socket,(LPSOCKADDR)&server_addr,sizeof(server_addr))){
		printf("server bind failed :d%",WSAGetLastError());
		exit(1);
	}
	//监听
	if(SOCKET_ERROR ==listen(m_Socket,10)){
		printf("Server listen failed :d%",WSAGetLastError());
		exit(1);
	}
	while(1){
		printf("Listening To client...\n");
		sockaddr_in client_addr;
		int client_addr_len = sizeof(client_addr);

		SOCKET  m_New_Socket = accept(m_Socket,(sockaddr *)&client_addr,&client_addr_len);
		if(SOCKET_ERROR==m_New_Socket){
			printf("Server Accept Failed :d%",WSAGetLastError());
			break;
		}
		char buffer[BUFFER_SIZE];
		memset(buffer,	0,BUFFER_SIZE);
		if(recv(m_New_Socket,buffer,BUFFER_SIZE,0)<0){
			printf("Server Receive Data Failed!");
			break;
		}else{
			cout<<"Server Receive Data success!"<<endl;
		}
		char file_name[FILE_NAME_MAX_SIZE+1];
		memset(file_name,0,FILE_NAME_MAX_SIZE+1);
		strncpy(file_name,buffer,strlen(buffer)>FILE_NAME_MAX_SIZE?FILE_NAME_MAX_SIZE:strlen(buffer));

		cout<<file_name<<endl;

		FILE *fp = fopen(file_name,"rb");//window 下是“rb”,表示打开一个只读的二进制文件
		if(NULL==fp){

			cout<<"not found file:"<<file_name<<endl;
		}else{
			memset(buffer,0,BUFFER_SIZE);
			int length = 0;
			while((length = fread(buffer,sizeof(char),BUFFER_SIZE,fp))>0){
				if(send(m_New_Socket,buffer,length,0)<0){
					printf("Send file:s% Failed\n",file_name);
					break;
				}
				memset(buffer,0,BUFFER_SIZE);
			}
			fclose(fp);
			printf("file:s% Transfer successed!\n",file_name);
		}
		closesocket(m_New_Socket);
	}
	closesocket(m_Socket);
	//释放winsock库
	return 0;
}

*/

