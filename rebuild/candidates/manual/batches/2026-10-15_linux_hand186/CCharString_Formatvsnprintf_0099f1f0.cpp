// CCharString::Format_vsnprintf(CCharString* result, const char* format, ...) -- printf-style formatting into a zeroed heap buffer (strlen(format) + 1024 bytes); on success the result is re-assigned from the text (left unassigned when empty)
// Leaf: Format_vsnprintf
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099F1F0, 176 bytes incl. padding (`__cdecl`, plain `ret`; the variadic
//          arguments start right after `format`).
// Callees: malloc @ 0x00BFEA0E / free @ 0x00BFEA14 (the scratch buffer), _vsnprintf @ 0x00BFEDAC, CCharString::UnassignString
//          @ 0x0099E9B0, operator new @ 0x00BFEA1A + CBasicString<char>::CBasicString(const char*) @ 0x009A0590 (new block, NoRefs = 1).
// Shape: strlen and memset are the VC7.1 intrinsics; the buffer allocation is null-guarded on a zero size; the last byte
//          is forced to NUL after the call; an over-long result (count > size) leaves the string untouched.
#include <string.h>
#include <stdarg.h>

extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
extern "C" int __cdecl _vsnprintf(char* buffer, unsigned int count, const char* format, va_list args);   // 0x00BFEDAC

#pragma pack(push, 1)
struct CBasicStringChar {                     // CBasicString<char>, 13 bytes
    char*         PData;                      // +0x00
    unsigned long StringLength;               // +0x04
    unsigned long DataLength    : 31;         // +0x08
    unsigned long m_reserved    : 1;          // +0x08 bit 31
    unsigned char UseFastExtend : 1;          // +0x0c bit 0
    CBasicStringChar(const char* text);       // 0x009A0590
};
struct CCharStringData {                      // 17 bytes
    CBasicStringChar Data;                    // +0x00
    long             NoRefs;                  // +0x0d
    CCharStringData(const char* text) : Data(text), NoRefs(1) { }
};
#pragma pack(pop)

class CCharString {
public:
    CCharStringData* PStringData;             // +0x00
    void UnassignString();                    // 0x0099E9B0
    static void __cdecl Format_vsnprintf(CCharString* result, const char* format, ...);
};

void __cdecl CCharString::Format_vsnprintf(CCharString* result, const char* format, ...)
{
    long size = strlen(format) + 0x400;
    char* buffer = size ? (char*)malloc(size) : 0;
    memset(buffer, 0, size);
    va_list args;
    va_start(args, format);
    int count = _vsnprintf(buffer, size, format, args);
    buffer[size - 1] = 0;
    if (count <= size) {
        if (result->PStringData)
            result->UnassignString();
        if (buffer == 0)
            return;
        if (*buffer)
            result->PStringData = new CCharStringData(buffer);
    }
    if (buffer)
        free(buffer);
}
