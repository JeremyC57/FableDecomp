// GFGetLastWindowsError() -- the system text for GetLastError(): FormatMessageW (ALLOCATE_BUFFER | FROM_SYSTEM) into a LocalAlloc'd wide buffer, narrowed through CWideString::ConvertToCharString, the buffer LocalFree'd; "" when FormatMessageW returns 0
// Leaf: ?GFGetLastWindowsError@@YI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5ED60, 136 bytes, __fastcall (the hidden CCharString result in ecx).
// Callees: GetLastError [0x0143FD58], FormatMessageW [0x0143FE7C], CWideString::CWideString(const wchar_t*) @ 0x0099B6B0,
//   CWideString::ConvertToCharString() const @ 0x0099B2C0 (straight into the named local), ~CWideString @ 0x0099B510,
//   LocalFree [0x0143FE78], CCharString(const CCharString&) @ 0x0099EC30 (no NRVO), ~CCharString @ 0x0099EAE0,
//   CCharString(const char*, long = -1) @ 0x0099EBF0 with "" @ 0x0122D70E.
// PDB: FableWin.pdb `?GFGetLastWindowsError@@YI?AVCCharString@@XZ`.
#include <stddef.h>

extern "C" __declspec(dllimport) unsigned long __stdcall GetLastError();
extern "C" __declspec(dllimport) unsigned long __stdcall FormatMessageW(unsigned long flags, const void* source,
    unsigned long messageId, unsigned long languageId, wchar_t* buffer, unsigned long size, char** arguments);
extern "C" __declspec(dllimport) void* __stdcall LocalFree(void* mem);

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);                       // 0x0099B6B0
    ~CWideString();                                         // 0x0099B510
    CCharString ConvertToCharString() const;                // 0x0099B2C0
};

CCharString __fastcall GFGetLastWindowsError()
{
    unsigned long error = GetLastError();
    wchar_t* buffer;
    if (FormatMessageW(0x1100, 0, error, 0, (wchar_t*)&buffer, 0, 0) > 0) {
        CCharString message = CWideString(buffer).ConvertToCharString();
        LocalFree(buffer);
        return message;
    }
    return CCharString("");
}