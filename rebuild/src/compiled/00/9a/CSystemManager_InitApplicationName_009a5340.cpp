// CSystemManager::InitApplicationName() -- take the executable name from the command line (first token, quotes stripped), cut it after the last '\\' and store it as the narrow ApplicationName
// Leaf: InitApplicationName
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A5340, 274 bytes, `ret`.
// Callees: GetCommandLineW (kernel32 import 0x0143FD74), wcsncpy 0x00BFEB04, wcslen 0x00BFED94, wcsrchr 0x00BFEDB2,
//   wcscpy 0x00BFED76 (CRT), CWideString::CWideString(const wchar_t*) 0x0099B6B0, CWideString::ConvertToCharString()
//   const 0x0099B2C0 (by value), CCharString::operator=(const CCharString&) 0x0099EFB0, ~CCharString 0x0099EAE0,
//   ~CWideString 0x0099B510, __security_check_cookie 0x00BFE9F9.
// Layout: ApplicationName (CCharString) at retail +0x168 (Ego_r.pdb +0x160; no engine header, local model).
// Shape: a /GS frame over two MAX_PATH+1 wide buffers (the name buffer zeroed by the memset intrinsic: rep stosd +
//   stosw); the token terminator lives in ebx (' ', or '"' for a quoted path) and is declared BEFORE the memset (that
//   schedules its `mov ebx,20h` between the wcsncpy count push and the trailing stosw; declared after the memset it
//   follows the stosw, declared at the quote test it sinks below the import call); a leading quote is removed by a
//   one-place copy loop VC turns into `rep movsw`; the token scan is a rotated while loop. The 8-aligned frame
//   (`and esp,-8`) comes with the two large buffers under /GS.
#include <stddef.h>
#include <string.h>
extern "C" __declspec(dllimport) wchar_t* __stdcall GetCommandLineW();          // 0x0143FD74 (import)
extern "C" wchar_t* __cdecl wcsncpy(wchar_t* dest, const wchar_t* src, size_t count);   // 0x00BFEB04
extern "C" size_t __cdecl wcslen(const wchar_t* text);                          // 0x00BFED94
extern "C" wchar_t* __cdecl wcsrchr(const wchar_t* text, wchar_t c);            // 0x00BFEDB2
extern "C" wchar_t* __cdecl wcscpy(wchar_t* dest, const wchar_t* src);          // 0x00BFED76

struct CCharString {
    void* PStringData;
    ~CCharString();                                          // 0x0099EAE0
    CCharString& operator=(const CCharString& other);        // 0x0099EFB0
};
struct CWideString {
    void* PStringData;
    CWideString(const wchar_t* text);                        // 0x0099B6B0
    ~CWideString();                                          // 0x0099B510
    CCharString ConvertToCharString() const;                 // 0x0099B2C0
};

class CSystemManager {
public:
    unsigned char _body[0x168];
    CCharString   ApplicationName;                           // +0x168
    void InitApplicationName();
};

void CSystemManager::InitApplicationName()
{
    wchar_t commandLine[261];
    wchar_t exeName[261];
    wchar_t terminator = L' ';
    memset(exeName, 0, sizeof(exeName));
    wcsncpy(commandLine, GetCommandLineW(), 261);
    if (commandLine[0] == L'"') {
        terminator = L'"';
        long length = wcslen(commandLine);
        for (long i = 0; i < length; ++i)
            commandLine[i] = commandLine[i + 1];
    }
    wchar_t* p = commandLine;
    while (*p && *p != terminator)
        ++p;
    *p = 0;
    wchar_t* slash = wcsrchr(commandLine, L'\\');
    if (slash) {
        wcscpy(exeName, slash + 1);
        ApplicationName = CWideString(exeName).ConvertToCharString();
    }
}