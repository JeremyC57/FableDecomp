// CAFile::SetCurrentPath(const CWideString& path) -- change the process working directory and remember it in CAFile's cached current path
// Leaf: ?SetCurrentPath@CAFile@@SIXABVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009974F0, length 30 bytes (+ int3 padding), static fastcall (path in ecx).
// Callees: CWideString::operator const wchar_t* @ 0x0099AF00, _wchdir @ 0x00BFEAFE,
// CWideString::operator=(const CWideString&) @ 0x0099B7D0 on the cached path @ 0x013BCA10 (also read by
// CAFile::GetCurrentPath @ 0x00997F80, which fills it from NFile::CalcCurrentPath when empty).
// Reference: also landed as rebuild/src/compiled/00/99/CAFile_SetCurrentPath_009974f0.cpp (shared-header form).
#include <stddef.h>

extern "C" int __cdecl _wchdir(const wchar_t* path);  // 0x00BFEAFE

class CWideString
{
public:
    operator const wchar_t*() const;                        // 0x0099AF00
    CWideString& operator=(const CWideString& other);       // 0x0099B7D0

private:
    void* Data;
};

class CAFile
{
public:
    static void __fastcall SetCurrentPath(const CWideString& path);

private:
    static CWideString CurrentPath;    // 0x013BCA10
};

void __fastcall CAFile::SetCurrentPath(const CWideString& path)
{
    _wchdir(path);
    CurrentPath = path;
}