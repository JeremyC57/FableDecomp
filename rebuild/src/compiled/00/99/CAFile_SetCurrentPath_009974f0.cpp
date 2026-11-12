// CAFile::SetCurrentPath(const CWideString& path) -- change the process working directory and remember it in NFile::CurrentPath
// Leaf: ?SetCurrentPath@CAFile@@SIXABVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009974F0, length 30 bytes (+ int3 padding), static fastcall (path in ecx).
// Callees: CWideString::operator const wchar_t* @ 0x0099AF00, _wchdir @ 0x00BFEAFE,
// CWideString::operator=(const CWideString&) @ 0x0099B7D0 on the cached path @ 0x013BCA10 (also read by
// CAFile::GetCurrentPath @ 0x00997F80, which fills it from NFile::CalcCurrentPath when empty).
// Reference: also landed as rebuild/src/compiled/00/99/CAFile_SetCurrentPath_009974f0.cpp (shared-header form).
// Name fix (agentNF26, FAM09): the cached path @ 0x013BCA10 is NFile::CurrentPath (Ego_r + FableWin dynamic initializer /
//   atexit destructor for NFile 'CurrentPath'; Anniv ?CurrentPath@NFile@@3VCWideString@@A), was the CAFile private static.
#include <stddef.h>

extern "C" int __cdecl _wchdir(const wchar_t* path);  // 0x00BFEAFE

class CWideString
{
public:
    operator const wchar_t*() const;                        // 0x0099AF00
    const CWideString& operator=(const CWideString& other);       // 0x0099B7D0

private:
    void* Data;
};

namespace NFile {
extern CWideString CurrentPath;    // 0x013BCA10 (Ego_r/FableWin NFile::`dynamic initializer for 'CurrentPath''; ?CurrentPath@NFile@@3VCWideString@@A)
}

class CAFile
{
public:
    static void __fastcall SetCurrentPath(const CWideString& path);
};

void __fastcall CAFile::SetCurrentPath(const CWideString& path)
{
    _wchdir(path);
    NFile::CurrentPath = path;
}