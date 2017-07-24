/*
 * server.cpp
 *
 *  Created on: 2017年7月21日
 *      Author: Administrator
 */
#define PORT 10004
#include <WinSock2.h>
#include <iostream>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <WINSOCK2.h>
#define BUFFER_SIZE 1024
using namespace std;
int main(){
	//声明并初始化一个服务端的地址结构
	 sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	server_addr.sin_port =htons(PORT);

	//初始化socket dll
	WSADATA wsaData;
	int nRet;
	if((nRet = WSAStartup(MAKEWORD(2,2),&wsaData)) != 0){
	cout<<"WSAStartup failed"<<endl;
	exit(0);
	}
	WORD socketVersion = MAKEWORD(2,0);
	if(WSAStartup(socketVersion,&wsaData)!=0){
		cout<<"Init socket dll error"<<endl;
		exit(1);
	}
	//创建socket
	SOCKET m_Socket = socket(AF_INET,SOCK_STREAM,0);
	if(SOCKET_ERROR == m_Socket){
		cout<<"Create Socket Error"<<endl;
		exit(1);
	}else{
		cout<<"Create Socket Success!"<<endl;
	}
	//绑定socket和服务端地址
	if(SOCKET_ERROR==bind(m_Socket,(LPSOCKADDR)&server_addr,sizeof(server_addr))){
		cout<<"Server Bind Failed:"<<WSAGetLastError()<<endl;
		exit(1);
	}else{
		cout<<"Server Bind Success!"<<endl;
	}
	//监听
	if(SOCKET_ERROR==listen(m_Socket,10)){
		cout<<"Server Listen Failed :"<<WSAGetLastError()<<endl;
		exit(1);
	}else{
		cout<<"Server Listen Success !"<<endl;
	}
	while(true){
		cout<<"wait for file transfer...\n"<<endl;
		char file_name[BUFFER_SIZE];
		char buffer [BUFFER_SIZE];
		sockaddr_in client_addr;
		int client_addr_len = sizeof(client_addr);
		//首先接收发过来的字符
		SOCKET m_New_Socket = accept(m_Socket,(sockaddr *)&client_addr,&client_addr_len);
		if(SOCKET_ERROR==m_New_Socket){
			cout<<"Server Accept Failed "<<WSAGetLastError()<<endl;
			break;
		}
		memset(buffer,0,sizeof(buffer));
		memset(file_name,0,sizeof(file_name));

		if(recv(m_New_Socket,buffer,sizeof(buffer),0)<0){
			cout<<"recv file name failed\n"<<endl;
			continue;
		}
		cout<<"recv file name:"<<file_name<<endl;
		cout<<"recv buffer name:"<<buffer<<endl;
		cout<<"\n"<<endl;
		strncpy(file_name,buffer,strlen(buffer));

	//	FILE * fp = fopen(file_name,"wb");
		FILE * fp = fopen("D://helloworld.txt","wb");
		if(fp==NULL){
			cout<<"open file error!"<<endl;
			continue;
		}
		//获取字符串后继续获取文件数据
		memset(buffer,0,BUFFER_SIZE);
		int length = 0;
		while((length = recv(m_New_Socket,buffer,BUFFER_SIZE,0))>0){
			if(fwrite(buffer,sizeof(char),length,fp)<length){

				cout<<"File；write failed "<<file_name<<endl;
				break;
			}
		memset(buffer,0,BUFFER_SIZE);
		}
		fclose(fp);
		cout<<"file transfer success!"<<endl;

	}
	system("pause");
	return 0;
}



