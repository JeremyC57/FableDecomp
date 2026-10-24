// NFile::CalcCurrentPath() -- read the process working directory (_wgetcwd into a MAX_PATH wide buffer) and return it as a CWideString
// Leaf: ?CalcCurrentPath@NFile@@YI?AVCWideString@@XZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009974A0, length 73 bytes (+ int3 padding), /GS frame (cookie 0x0139C8A8,
// __security_check_cookie 0x00BFE9F9), fastcall with the hidden result pointer in ecx.
// Callees: _wgetcwd @ 0x00BFED40, CWideString::CWideString(const wchar_t*) @ 0x0099B6B0.
// Caller: CAFile::GetCurrentPath @ 0x00997F80 (fills the cached CAFile current path @ 0x013BCA10 when it is empty).
// Label: the manifest's "AddManualCameraEvent" is wrong; Ego_r.pdb orders ?CalcCurrentPath@NFile@@ directly before
// ?SetCurrentPath@CAFile@@ (this row's retail neighbour 0x009974F0).
#include <stddef.h>

extern "C" wchar_t* __cdecl _wgetcwd(wchar_t* buffer, int maxLength);  // 0x00BFED40

class CWideString
{
public:
    CWideString(const wchar_t* text);   // 0x0099B6B0
    ~CWideString();

private:
    void* Data;
};

namespace NFile
{
    CWideString __fastcall CalcCurrentPath();
}

CWideString __fastcall NFile::CalcCurrentPath()
{
    wchar_t path[260];
    _wgetcwd(path, 260);
    return CWideString(path);
}