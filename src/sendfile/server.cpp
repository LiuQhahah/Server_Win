/*
 * server.cpp
 * IOT2000文件传输的服务端源码
 *  Created on: 2017年7月21日
 *      Author: Administrator
 */

#include <WinSock2.h>
#include <iostream>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <WINSOCK2.h>
#define PORT 10004//端口号
#define BUFFER_SIZE 1024//缓存器大小
using namespace std;
int main(){
	static int frequency = 0;
	//声明并初始化一个服务端的地址结构
	//用结构体处理网络通信的地址：sockaddr，sockaddr_in
	//网络中的地址包含3个方面的属性
	//1，地址类型：IPV4、IPV6；2，ip地址；3，端口
	//INADDR_ANY,转换过来就是0.0.0.0，泛指本机的意思，也就是表示本机的所有IP，
	//http://blog.csdn.net/jeffasd/article/details/51461568
	//因为有些机子不止一块网卡，多网卡的情况下，这个就表示所有网卡ip地址的意思。
	//AF_INET（又称 PF_INET）是 IPv4 网络协议的套接字类型，AF_INET6 则是 IPv6 的
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	server_addr.sin_port =htons(PORT);

	//初始化socket dll
	//大凡在WIN32平台上的WINSOCK编程都要经过下列步骤:
	//定义变量->获得WINDOCK版本->加载WINSOCK库->初始化->
	//创建套接字->设置套接字选项->关闭套接字->卸载WINSOCK库->释放资源
	WSADATA wsaData;
	int nRet;

	if((nRet = WSAStartup(MAKEWORD(2,2),&wsaData)) != 0){
	cout<<"WSAStartup failed"<<endl;
	exit(0);
	}

	//WSADATA WsaData;
	//WSAStartup(MAKEWORD(2,0),&WsaData);
	//根据版本通知操作系统,启用SOCKET的DLL库
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
	//listen 之前先绑定用于确保socket能在某个固定的端口监听
	//服务器端需要绑定，客户端不需要
	//绑定socket和服务端地址
	if(SOCKET_ERROR==bind(m_Socket,(LPSOCKADDR)&server_addr,sizeof(server_addr))){
		cout<<"Server Bind Failed:"<<WSAGetLastError()<<endl;
		exit(1);
	}else{
		cout<<"Server Bind Success!"<<endl;
	}
	//监听，10：客户端请求队列的上限http://blog.csdn.net/zhangzheng0413/article/details/8188967
	if(SOCKET_ERROR==listen(m_Socket,10)){
		cout<<"Server Listen Failed :"<<WSAGetLastError()<<endl;
		exit(1);
	}else{
		cout<<"Server Listen Success !"<<endl;
	}
	//true :一直接收，死循环
	while(true){
		cout<<"wait for file transfer...\n"<<endl;
		char file_name[BUFFER_SIZE];
		char buffer [BUFFER_SIZE];
		sockaddr_in client_addr;
		int client_addr_len = sizeof(client_addr);
		//接收发过来的字符
		SOCKET m_New_Socket = accept(m_Socket,(sockaddr *)&client_addr,&client_addr_len);
		if(SOCKET_ERROR==m_New_Socket){
			cout<<"Server Accept Failed "<<WSAGetLastError()<<endl;
			break;
		}
		//memset作用：对较大结构体或数组进行清零操作的一种最快方式，在一段内存块中填充某个给定的值
		//现将buffer用null占据空间，避免在strncpy填充字符时出现栈溢出情况！！！
		//http://blog.chinaunix.net/uid-23062171-id-4362652.html( strcpy和strncpy用法及区别 )
		memset(buffer,0,sizeof(buffer));
		memset(file_name,0,sizeof(file_name));

		//作用：从TCP的另一端接收数据，接收端套接字描述符:m_New_Socket
		//缓冲区用来存放recv函数接收到的数据:buffer
		//指明buffer的长度:sizeof(buffer)
		//第四个参数一般为0
		//http://www.cnblogs.com/jianqiang2010/archive/2010/08/20/1804598.html(Socket send函数和recv函数详解)
		if(recv(m_New_Socket,buffer,sizeof(buffer),0)<0){
			cout<<"recv file name failed\n"<<endl;
			continue;
		}
		cout<<"recv file name:"<<file_name<<endl;
		cout<<"recv buffer name:"<<buffer<<endl;
		cout<<"\n"<<endl;
		//凑够了第三个参数(strlen(buffer))的值才开始复制到file_name中
		//http://blog.chinaunix.net/uid-23062171-id-4362652.html( strcpy和strncpy用法及区别 )
		strncpy(file_name,buffer,strlen(buffer));

	//	FILE * fp = fopen(file_name,"wb");
		//第一个参数表路径与文件名，第二个参数为打开文件类型
		FILE * fp = fopen("D://helloworld.txt","wb");
		if(fp==NULL){
			cout<<"open file error!"<<endl;
			continue;
		}
		//获取字符串后继续获取文件数据
		//分配buffer空间为1024字节
		memset(buffer,0,BUFFER_SIZE);
		int length = 0;
		//????再次recv
		while((length = recv(m_New_Socket,buffer,BUFFER_SIZE,0))>0){
			if(fwrite(buffer,sizeof(char),length,fp)<length){
				cout<<"File write failed "<<file_name<<endl;
				break;
			}else{
				//接收内容
				cout<<"receive:"<<buffer<<endl;
			}
		memset(buffer,0,BUFFER_SIZE);
		}
		fclose(fp);
		cout<<"file transfer success!"<<endl;
		cout<<"共传输"<<frequency++<<"次!"<<endl;
	}
	system("pause");
	return 0;
}



