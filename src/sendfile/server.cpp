/*
 * server.cpp
 * IOT2000�ļ�����ķ����Դ��
 *  Created on: 2017��7��21��
 *      Author: Administrator
 */

#include <WinSock2.h>
#include <iostream>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <WINSOCK2.h>
#define PORT 10004//�˿ں�
#define BUFFER_SIZE 1024//��������С
using namespace std;
int main(){
	static int frequency = 0;
	//��������ʼ��һ������˵ĵ�ַ�ṹ
	//�ýṹ�崦������ͨ�ŵĵ�ַ��sockaddr��sockaddr_in
	//�����еĵ�ַ����3�����������
	//1����ַ���ͣ�IPV4��IPV6��2��ip��ַ��3���˿�
	//INADDR_ANY,ת����������0.0.0.0����ָ��������˼��Ҳ���Ǳ�ʾ����������IP��
	//http://blog.csdn.net/jeffasd/article/details/51461568
	//��Ϊ��Щ���Ӳ�ֹһ��������������������£�����ͱ�ʾ��������ip��ַ����˼��
	//AF_INET���ֳ� PF_INET���� IPv4 ����Э����׽������ͣ�AF_INET6 ���� IPv6 ��
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	server_addr.sin_port =htons(PORT);

	//��ʼ��socket dll
	//����WIN32ƽ̨�ϵ�WINSOCK��̶�Ҫ�������в���:
	//�������->���WINDOCK�汾->����WINSOCK��->��ʼ��->
	//�����׽���->�����׽���ѡ��->�ر��׽���->ж��WINSOCK��->�ͷ���Դ
	WSADATA wsaData;
	int nRet;

	if((nRet = WSAStartup(MAKEWORD(2,2),&wsaData)) != 0){
	cout<<"WSAStartup failed"<<endl;
	exit(0);
	}

	//WSADATA WsaData;
	//WSAStartup(MAKEWORD(2,0),&WsaData);
	//���ݰ汾֪ͨ����ϵͳ,����SOCKET��DLL��
	WORD socketVersion = MAKEWORD(2,0);
	if(WSAStartup(socketVersion,&wsaData)!=0){
		cout<<"Init socket dll error"<<endl;
		exit(1);
	}

	//����socket
	SOCKET m_Socket = socket(AF_INET,SOCK_STREAM,0);
	if(SOCKET_ERROR == m_Socket){
		cout<<"Create Socket Error"<<endl;
		exit(1);
	}else{
		cout<<"Create Socket Success!"<<endl;
	}
	//listen ֮ǰ�Ȱ�����ȷ��socket����ĳ���̶��Ķ˿ڼ���
	//����������Ҫ�󶨣��ͻ��˲���Ҫ
	//��socket�ͷ���˵�ַ
	if(SOCKET_ERROR==bind(m_Socket,(LPSOCKADDR)&server_addr,sizeof(server_addr))){
		cout<<"Server Bind Failed:"<<WSAGetLastError()<<endl;
		exit(1);
	}else{
		cout<<"Server Bind Success!"<<endl;
	}
	//������10���ͻ���������е�����http://blog.csdn.net/zhangzheng0413/article/details/8188967
	if(SOCKET_ERROR==listen(m_Socket,10)){
		cout<<"Server Listen Failed :"<<WSAGetLastError()<<endl;
		exit(1);
	}else{
		cout<<"Server Listen Success !"<<endl;
	}
	//true :һֱ���գ���ѭ��
	while(true){
		cout<<"wait for file transfer...\n"<<endl;
		char file_name[BUFFER_SIZE];
		char buffer [BUFFER_SIZE];
		sockaddr_in client_addr;
		int client_addr_len = sizeof(client_addr);
		//���շ��������ַ�
		SOCKET m_New_Socket = accept(m_Socket,(sockaddr *)&client_addr,&client_addr_len);
		if(SOCKET_ERROR==m_New_Socket){
			cout<<"Server Accept Failed "<<WSAGetLastError()<<endl;
			break;
		}
		//memset���ã��Խϴ�ṹ�������������������һ����췽ʽ����һ���ڴ�������ĳ��������ֵ
		//�ֽ�buffer��nullռ�ݿռ䣬������strncpy����ַ�ʱ����ջ������������
		//http://blog.chinaunix.net/uid-23062171-id-4362652.html( strcpy��strncpy�÷������� )
		memset(buffer,0,sizeof(buffer));
		memset(file_name,0,sizeof(file_name));

		//���ã���TCP����һ�˽������ݣ����ն��׽���������:m_New_Socket
		//�������������recv�������յ�������:buffer
		//ָ��buffer�ĳ���:sizeof(buffer)
		//���ĸ�����һ��Ϊ0
		//http://www.cnblogs.com/jianqiang2010/archive/2010/08/20/1804598.html(Socket send������recv�������)
		if(recv(m_New_Socket,buffer,sizeof(buffer),0)<0){
			cout<<"recv file name failed\n"<<endl;
			continue;
		}
		cout<<"recv file name:"<<file_name<<endl;
		cout<<"recv buffer name:"<<buffer<<endl;
		cout<<"\n"<<endl;
		//�չ��˵���������(strlen(buffer))��ֵ�ſ�ʼ���Ƶ�file_name��
		//http://blog.chinaunix.net/uid-23062171-id-4362652.html( strcpy��strncpy�÷������� )
		strncpy(file_name,buffer,strlen(buffer));

	//	FILE * fp = fopen(file_name,"wb");
		//��һ��������·�����ļ������ڶ�������Ϊ���ļ�����
		FILE * fp = fopen("D://helloworld.txt","wb");
		if(fp==NULL){
			cout<<"open file error!"<<endl;
			continue;
		}
		//��ȡ�ַ����������ȡ�ļ�����
		//����buffer�ռ�Ϊ1024�ֽ�
		memset(buffer,0,BUFFER_SIZE);
		int length = 0;
		//????�ٴ�recv
		while((length = recv(m_New_Socket,buffer,BUFFER_SIZE,0))>0){
			if(fwrite(buffer,sizeof(char),length,fp)<length){
				cout<<"File write failed "<<file_name<<endl;
				break;
			}else{
				//��������
				cout<<"receive:"<<buffer<<endl;
			}
		memset(buffer,0,BUFFER_SIZE);
		}
		fclose(fp);
		cout<<"file transfer success!"<<endl;
		cout<<"������"<<frequency++<<"��!"<<endl;
	}
	system("pause");
	return 0;
}



