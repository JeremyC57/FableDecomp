// CDriveManager::DriveExists(wchar_t driveLetter) -- probe a drive by changing into its root ("X:\"): 3 when that fails, else restore the previous working directory and return 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E4AE0, length 138 bytes (+ int3 padding). Flags: /GS (security cookie @ 0x0139C8A8).
// Callees: GetCurrentWorkingDirectory @ 0x00997F80 (`__fastcall`, the CWideString result through the hidden pointer in ecx, no arguments; the
// manifest label "CInputProcessMain_GetCameraPath" is propagated), _wchdir @ 0x00BFEAFE (CRT, __cdecl), CWideString::operator const wchar_t*
// @ 0x0099AF00, CWideString::~CWideString @ 0x0099B510 (once per return path), __security_check_cookie @ 0x00BFE9F9.
// `this` is unused (`ret 4`); the engine header CDriveManager.h declares the two words but no methods, so the class is modelled locally with
// the same layout (as the landed CDriveManager::DeleteFile 0x009E4BC0). wchar_t is `unsigned short` under the era defaults.
// Shape: a 0x10-byte /GS frame -- the saved working directory at +0, the four-character root string at +4, the cookie at +0xc; the letter
// is loaded as a word and the string completed with immediate word stores (':', '\\', 0) after its address is pushed; `jne` on the
// _wchdir result leads to the failure tail (destructor, `mov eax,3`) placed last, the success path (the `if` body) changes back and returns
// 1 first; the `!= 0` early-return form inverts the block order.
// Flags: /GS
typedef unsigned short wchar16;

extern "C" int __cdecl _wchdir(const wchar16* path);   // 0x00BFEAFE

struct CWideString {
    wchar16* m_pchData;
    operator const wchar16*();               // 0x0099AF00
    ~CWideString();                          // 0x0099B510
};
CWideString __fastcall GetCurrentWorkingDirectory();   // 0x00997F80

struct CSystemManager;

struct CDriveManager {
    void*           __vftable;               // +0x00
    CSystemManager* SystemManager;           // +0x04
    long DriveExists(wchar16 driveLetter);
};

long CDriveManager::DriveExists(wchar16 driveLetter)
{
    CWideString workingDirectory = GetCurrentWorkingDirectory();
    wchar16 root[4];
    root[0] = driveLetter;
    root[1] = 0x3a;                          // ':'
    root[2] = 0x5c;                          // '\\'
    root[3] = 0;
    if (_wchdir(root) == 0) {
        _wchdir(workingDirectory);
        return 1;
    }
    return 3;
}
