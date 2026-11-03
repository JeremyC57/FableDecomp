// CDriveManager::DeleteFileW(const CWideString&) -- (the chunk label repeats "DeleteDirectory"; the import at 0x0143FE34 is DeleteFileW) delete the file at the full path of the given path; on failure read the last error and report 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E4BC0, length 70 bytes.
// Leaf: ?DeleteFileW@CDriveManager@@QAE?AW4EDriveReturn@@ABVCWideString@@@Z
// Callees: ConvertPathToFullPath @ 0x00999110 (`__fastcall`: the CWideString result through the hidden pointer in ecx, the path reference in edx), CWideString::operator
// const wchar_t* @ 0x0099AF00 (manifest "CCharString::operator char const*" is propagated), DeleteFileW (import 0x0143FE34), GetLastError (import
// 0x0143FD58; its result is dropped -- the call survives), CWideString::~CWideString @ 0x0099B510 (once per return path).
// `this` is unused (`ret 4` with the argument at [esp+4]); the class is engine/CDriveManager.h's FABLE_RETAIL_CDRIVEMANAGER branch. Shape: the full-path temporary is homed in the dead argument slot (`lea ecx,[esp+4]`), the result
// is an EDriveReturn (`mov eax,1` / `xor eax,eax`).
#include <stddef.h>
extern "C" __declspec(dllimport) int __stdcall DeleteFileW(const wchar_t* path);   // 0x0143FE34
extern "C" __declspec(dllimport) unsigned long __stdcall GetLastError();                 // 0x0143FD58

class CWideString { public:
    wchar_t* m_pchData;
    operator const wchar_t*() const;               // 0x0099AF00
    ~CWideString();                                // 0x0099B510
};
class CAFile {
public:
    static CWideString __fastcall ConvertPathToFullPath(const CWideString&);   // 0x00999110
};   // 0x00999110

// Name (landing 2026-11-03e, chunk 734 NAME_FIXES): Ego_r.pdb + FableWin.pdb ?DeleteFileW@CDriveManager@@QAE?AW4EDriveReturn@@ABVCWideString@@@Z --
// the source wrote DeleteFile and <windows.h>'s macro renamed the member and the API call alike; the call is the global import (`::`).
// EDriveReturn is the PDB return type (enumerator names are not in the PDB name sets: the values retail returns are written as casts).
#define FABLE_RETAIL_CDRIVEMANAGER
#include "engine/CDriveManager.h"

EDriveReturn CDriveManager::DeleteFileW(const CWideString& path)
{
    CWideString fullPath = CAFile::ConvertPathToFullPath(path);
    if (::DeleteFileW(fullPath))
        return (EDriveReturn)1;
    GetLastError();
    return (EDriveReturn)0;
}