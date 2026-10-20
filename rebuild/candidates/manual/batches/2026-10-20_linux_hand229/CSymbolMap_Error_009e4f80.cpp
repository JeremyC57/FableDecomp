// CSymbolMap::Error(const CWideString& file, long line, const char* format, ...) -- format a compiler-style "<file>(<line>) : error C2065 : <message>" string (the 0-based line printed 1-based); the text is built and dropped (its consumer is compiled out of the retail build)
// Leaf: Error
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E4F80, 270 bytes (+2 int3 padding), `ret` (a varargs member: __cdecl with
//   `this` as the first stack argument, never read). Callers 0x009E570C.. (CSymbolMap's definition parser).
// Callees: CCharString::CCharString() 0x0099E4B0, _snprintf 0x00BFEB96, vsprintf 0x00BFEDD0,
//   CWideString::ConvertToCharString() const 0x0099B2C0 (by value), CCharString(const char*, long) 0x0099EBF0,
//   operator+(const CCharString&, const CCharString&) 0x0099F570 / operator+(const CCharString&, const char*) 0x0099F600
//   (__fastcall: result in ecx, left in edx), CCharString::operator=(const CCharString&) 0x0099EFB0, ~CCharString
//   0x0099EAE0, __security_check_cookie 0x00BFE9F9. Literals "(%d)" 0x0129C14C, " : error C2065 : " 0x0129C138,
//   "" 0x0122D70E.
// Shape: a /GS frame (cookie at +0x128): the result string is a named local constructed first and assigned last; the
//   line buffer is _snprintf'd with a count of 9 and terminated at [9]; the message is vsprintf'd from the varargs; the
//   concatenation starts from an explicit CCharString("", -1) temporary (VC evaluates the right operand, the converted
//   file name, first; every temporary dies at the end of the full expression, in reverse order).
#include <stdarg.h>
extern "C" int __cdecl _snprintf(char* buffer, unsigned int count, const char* format, ...);   // 0x00BFEB96
extern "C" int __cdecl vsprintf(char* buffer, const char* format, va_list args);              // 0x00BFEDD0

struct CCharString {
    void* PStringData;
    CCharString();                                           // 0x0099E4B0
    CCharString(const char* text, long length);              // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    CCharString& operator=(const CCharString& other);        // 0x0099EFB0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600

struct CWideString {
    void* PStringData;
    CCharString ConvertToCharString() const;                 // 0x0099B2C0
};

class CSymbolMap {
public:
    void Error(const CWideString& file, long line, const char* format, ...);
};

void CSymbolMap::Error(const CWideString& file, long line, const char* format, ...)
{
    CCharString errorText;
    char lineText[12];
    char message[256];
    _snprintf(lineText, 9, "(%d)", line + 1);
    lineText[9] = 0;
    va_list args;
    va_start(args, format);
    vsprintf(message, format, args);
    errorText = CCharString("", -1) + file.ConvertToCharString() + lineText + " : error C2065 : " + message;
}
