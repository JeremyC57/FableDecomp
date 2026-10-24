// CNamedRecord_00841e90::CNamedRecord_00841e90() -- an STLport string (default-constructed inline:
// Leaf: ??0CNamedRecord_00841e90@
// the three pointers nulled by the _String_base part, then an 8-byte malloc block, empty and
// terminated) that is then assigned the empty literal in the body, followed by four scalar
// members cleared in the body (init-list zeros would be stored before the assign call)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00841E90, length 72 bytes (+ int3 padding).
// Callees: malloc @ 0x00BFEA0E (the string's 8-byte initial block), basic_string::assign(const
// char* first, const char* last) @ 0x005693B0 (the manifest's "Vector_Insert" label is
// propagated) with the empty literal @ 0x0122D70E as both bounds (strlen("") folded to 0, so the
// same pointer is pushed twice).
// Layout: {char* start, char* finish, char* end_of_storage} +0x00..+0x0c, long +0x0c, short
// +0x10, long +0x14, long +0x18 (0x1c bytes). The zero register ebx also terminates the block.
// The end-of-storage pointer is STLport's _STLP_alloc_proxy member (its own inline constructor),
// which is why the `push 8` of the allocation is scheduled between the second and third null
// store; the four scalars are assigned in the body in the order short, long, long, long.
// The manifest label "CContainedBank::CContainedBank" is rejected: the PDB CContainedBank is a
// 20-byte POD of unsigned longs with no string. Class named by shape.
#include <string.h>
extern "C" void* __cdecl malloc(size_t size);   // 0x00BFEA0E

struct SRetailAllocProxy {                      // STLport _STLP_alloc_proxy<char*, char, allocator>: the end-of-storage pointer
    char* m_data;
    SRetailAllocProxy(char* data) : m_data(data) { }
};

struct CRetailStringBase {                      // STLport _String_base: start / finish nulled, the proxy member nulled, then the block allocated
    char* m_start;
    char* m_finish;
    SRetailAllocProxy m_endOfStorage;
    CRetailStringBase(size_t n) : m_start(0), m_finish(0), m_endOfStorage(0)
    {
        m_start = static_cast<char*>(malloc(n));
        m_finish = m_start;
        m_endOfStorage.m_data = m_start + n;
    }
};

struct CRetailString : CRetailStringBase {      // STLport basic_string<char>: 8-byte initial block, terminated
    CRetailString() : CRetailStringBase(8)
    {
        *m_finish = 0;
    }
    CRetailString& Assign(const char* first, const char* last);   // 0x005693B0
    CRetailString& operator=(const char* text) { return Assign(text, text + strlen(text)); }
};

struct CNamedRecord_00841e90 {
    CRetailString m_name;                       // +0x00
    long          m_0c;                         // +0x0c
    short         m_10;                         // +0x10
    long          m_14;                         // +0x14
    long          m_18;                         // +0x18
    CNamedRecord_00841e90();
};

CNamedRecord_00841e90::CNamedRecord_00841e90()
    : m_name()
{
    m_name = "";
    m_10 = 0;
    m_0c = 0;
    m_14 = 0;
    m_18 = 0;
}