// CDriveManager::DeleteFile(const CWideString&) -- (the chunk label repeats "DeleteDirectory"; the import at 0x0143FE34 is DeleteFileW) delete the file at the full path of the given path; on failure read the last error and report 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E4BC0, length 70 bytes.
// Callees: ConvertPathToFullPath @ 0x00999110 (`__fastcall`: the CWideString result through the hidden pointer in ecx, the path reference in edx), CWideString::operator
// const wchar_t* @ 0x0099AF00 (manifest "CCharString::operator char const*" is propagated), DeleteFileW (import 0x0143FE34), GetLastError (import
// 0x0143FD58; its result is dropped -- the call survives), CWideString::~CWideString @ 0x0099B510 (once per return path).
// `this` is unused (`ret 4` with the argument at [esp+4]); the engine header CDriveManager.h declares the two words but no methods, so the class
// is modelled locally with the same layout. Shape: the full-path temporary is homed in the dead argument slot (`lea ecx,[esp+4]`), the result
// is a long (`mov eax,1` / `xor eax,eax`).
#include <stddef.h>
extern "C" __declspec(dllimport) int __stdcall DeleteFileW(const wchar_t* path);   // 0x0143FE34
extern "C" __declspec(dllimport) unsigned long __stdcall GetLastError();                 // 0x0143FD58

class CWideString { public:
    wchar_t* m_pchData;
    operator const wchar_t*();                     // 0x0099AF00
    ~CWideString();                                // 0x0099B510
};
CWideString __fastcall ConvertPathToFullPath(const CWideString& path);   // 0x00999110

class CSystemManager;

class CDriveManager { public:
    void*           __vftable;                     // +0x00
    CSystemManager* SystemManager;                 // +0x04
    long DeleteFile(const CWideString& path);
};

long CDriveManager::DeleteFile(const CWideString& path)
{
    CWideString fullPath = ConvertPathToFullPath(path);
    if (DeleteFileW(fullPath))
        return 1;
    GetLastError();
    return 0;
}