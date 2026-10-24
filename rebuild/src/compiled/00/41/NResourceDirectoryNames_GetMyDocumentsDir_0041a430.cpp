// NResourceDirectoryNames::GetMyDocumentsDir() -- the user's My Documents folder (SHGetFolderPathW CSIDL_PERSONAL) with a trailing backslash guaranteed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041A430, 141 bytes (row padded to 144). FableWin.pdb:
// ?GetMyDocumentsDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ (`__fastcall` free function: hidden result in ecx, `ret`).
// Callees: the SHGetFolderPathW import thunk slot [0x01440280] (called through the IAT, `__stdcall`, five arguments),
// CWideString(const wchar_t*) @ 0x0099B6B0, GetLength @ 0x0099B220, operator const wchar_t* @ 0x0099AF10,
// operator+=(const wchar_t*) @ 0x0099B940 (literal L"\\" @ 0x0122DE78), CWideString(const CWideString&) @ 0x0099B720,
// ~CWideString @ 0x0099B510, __security_check_cookie @ 0x00BFE9F9 (cookie 0x0139C8A8).
// Build: /GS (the TU is built with buffer-security checks: cookie stored above the 260-character path buffer).
// Shape: 0x210-byte frame = the CWideString local (4) + wchar_t path[260] (0x208) + the cookie (4); VC7.1 has no NRVO, so
// the named local is copy-constructed into the hidden result and destroyed. The behaviour matches the bootstrap fixture
// copy at rebuild/src/compiled/00/41/NResourceDirectoryNames_GetMyDocumentsDir_0041a430.cpp (which calls the import
// through a descriptive function-pointer global); this source names the real import.
// Flags: /GS
#include "fable_string.h"

extern "C" __declspec(dllimport) long __stdcall SHGetFolderPathW(void* window, int folder, void* token,
                                                                 unsigned long flags, wchar_t* path);   // IAT 0x01440280

namespace NResourceDirectoryNames {
CWideString __fastcall GetMyDocumentsDir()
{
    wchar_t path[260];
    SHGetFolderPathW(0, 5, 0, 0, path);            // CSIDL_PERSONAL
    CWideString directory(path);
    long lastIndex = directory.GetLength() - 1;
    const wchar_t* text = directory;
    if (text[lastIndex] != L'\\')
        directory += L"\\";
    return directory;
}
}