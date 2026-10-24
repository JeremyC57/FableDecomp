// NResourceDirectoryNames::GetWriteableEngineCacheDir() -- the per-user writeable engine cache folder: <Application Data>\Microsoft\Fable\ (SHGetFolderPathW CSIDL_APPDATA)
// Leaf: ?GetWriteableEngineCacheDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041A220, length 99 bytes (row padded to 112). Ego_r.pdb / FableWin.pdb:
// ?GetWriteableEngineCacheDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ (`__fastcall` free function: hidden result
// in ecx, `ret`). Callers: 0x0042E2EA, 0x00435278.
// Callees: the SHGetFolderPathW import slot [0x01440280] (`__stdcall`, five arguments; folder 0x1A = CSIDL_APPDATA),
// CWideString(const wchar_t*) @ 0x0099B6B0, operator+(const CWideString&, const wchar_t*) @ 0x0099BF30 (`__fastcall`:
// hidden result in ecx, left in edx, the literal L"\\Microsoft\\Fable\\" @ 0x0122F4B0 on the stack),
// ~CWideString @ 0x0099B510, __security_check_cookie @ 0x00BFE9F9 (cookie 0x0139C8A8).
// Build: /GS, like its TU neighbour GetMyDocumentsDir (0x0041A430, landed): the cookie sits above the 260-character path.
// Shape: 0x210-byte frame = the CWideString(path) temporary (4) + wchar_t path[260] (0x208) + the cookie (4); the sum is
// built straight into the hidden result (no named local, so no copy), then the temporary is destroyed.
// Flags: /GS
#include "fable_string.h"

extern "C" __declspec(dllimport) long __stdcall SHGetFolderPathW(void* window, int folder, void* token,
                                                                 unsigned long flags, wchar_t* path);   // IAT 0x01440280

namespace NResourceDirectoryNames {
CWideString __fastcall GetWriteableEngineCacheDir()
{
    wchar_t path[260];
    SHGetFolderPathW(0, 0x1a, 0, 0, path);         // CSIDL_APPDATA
    return CWideString(path) + L"\\Microsoft\\Fable\\";
}
}