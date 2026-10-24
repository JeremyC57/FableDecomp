// CDateAndTime::ToString() const -- format the date and time as "[dd/mm/yy hh:mm:ss]" (day, month, year, hour, minute, second) into a zeroed 1 KB heap buffer and return it as a CCharString
// Leaf: ?ToString@CDateAndTime
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E9E30, length 125 bytes (`ret 4`: the hidden result pointer). PDB:
//   ?ToString@CDateAndTime@@QBE?AVCCharString@@XZ; CDateAndTime = {long Year; ulong Month, Day, Hour, Minute, Second, Millisecond}.
// Label correction: the catalog's "Format_DateTimeString" is CDateAndTime::ToString.
// Callees: malloc @ 0x00BFEA0E, sprintf @ 0x00BFEB10 (format @ 0x0129C3A8), CCharString(const char*, long) @ 0x0099EBF0,
//   the CCharString copy constructor @ 0x0099EC30, ~CCharString @ 0x0099EAE0, free @ 0x00BFEA14.
// Shape: the buffer is freed AFTER the result copy and the local string's destructor, so it is owned by a local object whose
//   inline destructor runs last (`if (p) free(p);`); its constructor mallocs and zero-fills the 1024 bytes (`rep stosd`,
//   0x100 dwords). The holder's real name is unknown (descriptive `CTextBuffer`). VC7.1 has no NRVO: the named CCharString in
//   the `push ecx` word is copied into the hidden result and destroyed.
#include <string.h>
extern "C" void* __cdecl malloc(unsigned int size);                   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                             // 0x00BFEA14
extern "C" int __cdecl sprintf(char* buffer, const char* format, ...); // 0x00BFEB10

class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

struct CTextBuffer {
    char* Data;
    CTextBuffer(unsigned int size) : Data((char*)malloc(size)) { memset(Data, 0, size); }
    ~CTextBuffer() { if (Data) free(Data); }
};

class CDateAndTime {
public:
    long          Year;                     // +0x00
    unsigned long Month;                    // +0x04
    unsigned long Day;                      // +0x08
    unsigned long Hour;                     // +0x0c
    unsigned long Minute;                   // +0x10
    unsigned long Second;                   // +0x14
    unsigned long Millisecond;              // +0x18
    CCharString ToString() const;
};

CCharString CDateAndTime::ToString() const
{
    CTextBuffer text(1024);
    sprintf(text.Data, "[%02d/%02d/%02d %02d:%02d:%02d]", Day, Month, Year, Hour, Minute, Second);
    CCharString result(text.Data);
    return result;
}