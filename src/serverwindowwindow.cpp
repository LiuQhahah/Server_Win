/*
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")

int main(int argc, char* argv[])
{
    //��ʼ��WSA
    WORD sockVersion = MAKEWORD(2,2);
    WSADATA wsaData;
    if(WSAStartup(sockVersion, &wsaData)!=0)
    {
        return 0;
    }

    //�����׽���
    SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(slisten == INVALID_SOCKET)
    {
        printf("socket error !");
        return 0;
    }

    //��IP�Ͷ˿�
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(10004);
    sin.sin_addr.S_un.S_addr = INADDR_ANY;
    if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        printf("bind error !");
    }

    //��ʼ����
    if(listen(slisten, 5) == SOCKET_ERROR)
    {
        printf("listen error !");
        return 0;
    }

    //ѭ����������
    SOCKET sClient;
    sockaddr_in remoteAddr;
    int nAddrlen = sizeof(remoteAddr);
    char revData[255];
    while (true)
    {

        cout<<"�ȴ�����..."<<endl;
        sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
        if(sClient == INVALID_SOCKET)
        {
            printf("accept error !");
            continue;
        }

        cout<<"���ܵ�һ�����ӣ�"<<inet_ntoa(remoteAddr.sin_addr)<<"\r\n"<<endl;
        //��������
        int ret = recv(sClient, revData, 255, 0);
        if(ret > 0)
        {
            revData[ret] = 0x00;

            cout<<"revData:"<<revData<<endl;
        }else{
        	cout<<ret<<"δ�յ����ݣ�"<<endl;
        }


        //��������
        const char * sendData = "��ã�TCP�ͻ��ˣ�\n";
        send(sClient, sendData, strlen(sendData), 0);
        closesocket(sClient);
    }

    closesocket(slisten);
    WSACleanup();
    return 0;
}
*/
