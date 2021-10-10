//#include<Windows.h>
#include"resource.h"
#include<iostream>
#include <WinSock2.h>
#include <Iphlpapi.h>
#include <string>
#include <map>
#include <vector>
#include <WS2tcpip.h>


#pragma comment(lib,"iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
INT_PTR CALLBACK dlgproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
HWND g_hwnd;
int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
) {

    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, dlgproc);

}

typedef struct _MyAdpterInfo
{
    std::string Ip;
    std::string MacAddress;
    std::string Gateway;
    std::string Name;

    UINT Type;
}MyAdpterInfo;



void GetAdainfo(const wchar_t* FriendName_, MyAdpterInfo& adpterInfo) {
    ULONG Sizepointer = 15000;
    IP_ADAPTER_ADDRESSES* ipadaadd = (IP_ADAPTER_ADDRESSES*)malloc(Sizepointer);
    ULONG errorCode = 0;
    errorCode = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_GATEWAYS | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_INCLUDE_TUNNEL_BINDINGORDER, NULL, ipadaadd, &Sizepointer);
    IP_ADAPTER_ADDRESSES* IpTmp = ipadaadd;
    while (IpTmp) {
        // 打印 适配器名称  友好
        //WCHAR FriendName[255] = { 0 };
        if (!wcscmp(IpTmp->FriendlyName, FriendName_)) {

            // IP 地址
            char IP[50] = { 0 };
            if (IpTmp->FirstUnicastAddress != NULL) {
                IpTmp->FirstUnicastAddress->Address.lpSockaddr->sa_data;
                inet_ntop(PF_INET, (byte*)(IpTmp->FirstUnicastAddress->Address.lpSockaddr->sa_data) + 2, IP, sizeof(IP));
            }
       
            // 网关IP 地址
            char Gateway[50] = { 0 };
            if (IpTmp->FirstGatewayAddress != NULL) {
                IpTmp->FirstGatewayAddress->Address.lpSockaddr->sa_data;
                inet_ntop(PF_INET, (byte*)(IpTmp->FirstGatewayAddress->Address.lpSockaddr->sa_data) + 2, Gateway, sizeof(Gateway));
            }
            // Mac地址 

            // 类型

            // 网卡名称


            //adpterInfo.Name = IpTmp->FriendlyName;
            adpterInfo.Ip = IP;
            adpterInfo.Gateway = Gateway;
            break;

        }
        //wcscpy_s(FriendName, IpTmp->FriendlyName);



        //printf("\n适配器名称: %ws, IP:%s, gateway:%s", FriendName, IP, Gateway);
        IpTmp = IpTmp->Next;
    }
    //printf("%x", errorCode);


    free(ipadaadd);
    ipadaadd = NULL;
    return;
}

void SetAdpinfo_Thread() {

    // 获取自身ip 地址，并填写进 程序里
    MyAdpterInfo adpterinfo;
    while (TRUE) {
        GetAdainfo(L"WLAN", adpterinfo);
        SetDlgItemTextA(g_hwnd, IDC_STATIC_GATEWAYVALUE, adpterinfo.Gateway.c_str());
        SetDlgItemTextA(g_hwnd, IDC_STATIC_IPVALUE, adpterinfo.Ip.c_str());
        Sleep(1000);
    }


}

INT_PTR CALLBACK dlgproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {



    // 消息循环
    switch (msg)
    {
    case WM_INITDIALOG: {
        g_hwnd = hwnd;
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SetAdpinfo_Thread, NULL, NULL, NULL);
    }
        break;

    case WM_PAINT:
        break;

    case WM_LBUTTONDOWN:
        //DestroyWindow(hwnd);
        break;

    default: {
        // 没处理过的消息

    }
    }

    return FALSE;
}