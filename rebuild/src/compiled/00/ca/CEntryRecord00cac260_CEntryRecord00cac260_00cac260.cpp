// CEntryRecord_00cac260::CEntryRecord_00cac260(long a, long b, long c, long d, long e) -- the 0x9c-byte base constructed with the five arguments and a zero, then a cleared word at +0x9c, two cleared bytes at +0xa0/+0xa1 and the twelve-word table at +0xa8 cleared with `rep stosd`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAC260, length 82 bytes.
// Callees: the base constructor @ 0x00CAC070 (the manifest label "InitializeEntry" is propagated; six arguments, the last a constant 0). Shape: the arguments are
// shuffled through eax/edx/ecx around `push esi ; push edi`, the word and bytes are immediates, the table clear uses the zero register through edi; `this` is
// returned, `ret 14h`. The chunk label "CEnginePrimitiveTypeInfo" is propagated (the 0x00CAxxxx module); the class is named by address.
#include <string.h>

struct CEntryBase_00cac070 {                       // 0x9c bytes
    unsigned char _body[0x9c];
    CEntryBase_00cac070(long a, long b, long c, long d, long e, long f);   // 0x00CAC070
};

struct CEntryRecord_00cac260 : CEntryBase_00cac070 {
    long          m_count;                         // +0x9c
    bool          m_flagA;                         // +0xa0
    bool          m_flagB;                         // +0xa1
    unsigned char _pada2[6];                       // +0xa2 .. +0xa8 (not initialised)
    long          m_table[12];                     // +0xa8 .. +0xd8
    CEntryRecord_00cac260(long a, long b, long c, long d, long e);
};

CEntryRecord_00cac260::CEntryRecord_00cac260(long a, long b, long c, long d, long e)
    : CEntryBase_00cac070(a, b, c, d, e, 0)
    , m_count(0), m_flagA(false), m_flagB(false)
{
    memset(m_table, 0, sizeof(m_table));           // the intrinsic: `rep stosd` of twelve words
}