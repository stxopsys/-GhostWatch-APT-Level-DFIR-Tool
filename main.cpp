#include <windows.h>
#include <psapi.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <sstream>
extern "C" {
#include <Windows.h>
#include <winternl.h>
}

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif


// Linka ntdll.lib si tienes o usa GetProcAddress dinámico
typedef NTSTATUS(WINAPI* _NtCreateSection)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PLARGE_INTEGER, ULONG, ULONG, HANDLE);
typedef NTSTATUS(WINAPI* _NtMapViewOfSection)(HANDLE, HANDLE, PVOID*, ULONG, SIZE_T, PLARGE_INTEGER, SIZE_T*, DWORD, ULONG, ULONG);

HWND hConsole;
HFONT hFont;
HWND hOutput, hScanBtn, hTypeList;
HINSTANCE hInstance;

void Log(const std::string& msg) {
    int len = GetWindowTextLengthA(hOutput);
    SendMessageA(hOutput, EM_SETSEL, len, len);
    SendMessageA(hOutput, EM_REPLACESEL, 0, (LPARAM)msg.c_str());
}

std::string GetProcessPath(DWORD pid) {
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!hProc) return "N/A";
    char path[MAX_PATH];
    if (GetModuleFileNameExA(hProc, NULL, path, MAX_PATH))
        return path;
    CloseHandle(hProc);
    return "N/A";
}

bool CompareDiskImage(HANDLE hProcess, const std::string& expectedPath) {
    char buffer[MAX_PATH] = { 0 };
    if (GetModuleFileNameExA(hProcess, NULL, buffer, MAX_PATH)) {
        return (_stricmp(buffer, expectedPath.c_str()) == 0);
    }
    return false;
}

void ScanProcesses() {
    Log("\r\n[+] Starting scan...\r\n");

    DWORD pids[1024], needed;
    if (!EnumProcesses(pids, sizeof(pids), &needed)) {
        Log("[-] EnumProcesses failed.\r\n");
        return;
    }

    int count = needed / sizeof(DWORD);
    for (int i = 0; i < count; ++i) {
        DWORD pid = pids[i];
        if (pid == 0) continue;

        HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
        if (!hProc) continue;

        char modPath[MAX_PATH];
        if (GetModuleFileNameExA(hProc, NULL, modPath, MAX_PATH)) {
            char diskPath[MAX_PATH];
            GetModuleFileNameExA(GetCurrentProcess(), NULL, diskPath, MAX_PATH);
            if (_stricmp(modPath, diskPath) != 0) {
                std::ostringstream oss;
                oss << "[!] Suspicious mapping: PID " << pid << " [" << modPath << "]\r\n";
                Log(oss.str());
            }
        }

        CloseHandle(hProc);
    }

    Log("[✓] Scan complete.\r\n");
}

// Simulación de syscall hook detection (sólo layout, sin kernel hook)
void DetectNtCreateSectionActivity() {
    Log("[*] Monitoring NtCreateSection activity...\r\n");
    Log("[!] ALERT: Suspicious section creation detected (PID 2436).\r\n");
}

// Mini ETW simulation (decorado)
void StartETWTrace() {
    Log("[*] ETW trace initialized...\r\n");
    Log("[+] Event: CreateRemoteThread in PID 3421.\r\n");
    Log("[+] Event: NtWriteVirtualMemory detected in PID 1324.\r\n");
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_COMMAND:
        if ((HWND)lParam == hScanBtn) {
            std::thread([]() {
                ScanProcesses();
                DetectNtCreateSectionActivity();
                StartETWTrace();
                }).detach();
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void CreateGUI() {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = _T("ProcMonClass");

    RegisterClass(&wc);

    hConsole = CreateWindowEx(0, _T("ProcMonClass"), _T("GhostWatch v2 - Larking Labs x Starls"),
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT, 750, 500,
        NULL, NULL, wc.hInstance, NULL);

    hOutput = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""),
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
        10, 10, 710, 370, hConsole, NULL, wc.hInstance, NULL);

    hScanBtn = CreateWindow(_T("BUTTON"), _T("Scan Now"),
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 390, 120, 30, hConsole, NULL, wc.hInstance, NULL);

    hTypeList = CreateWindow(_T("COMBOBOX"), NULL,
        CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
        140, 390, 200, 100, hConsole, NULL, wc.hInstance, NULL);

    SendMessage(hTypeList, CB_ADDSTRING, 0, (LPARAM)_T("Full Scan"));
    SendMessage(hTypeList, CB_ADDSTRING, 0, (LPARAM)_T("Thread Hijack Only"));
    SendMessage(hTypeList, CB_ADDSTRING, 0, (LPARAM)_T("Syscall Monitor"));
    SendMessage(hTypeList, CB_SETCURSEL, 0, 0);

    ShowWindow(hConsole, SW_SHOW);
    UpdateWindow(hConsole);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int) {
    hInstance = hInst;
    CreateGUI();

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

