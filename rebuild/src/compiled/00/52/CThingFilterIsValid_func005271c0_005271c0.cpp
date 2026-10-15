// CThingFilter_IsValid::func_005271c0(bool, const CCharString&, long, long, bool, const CCharString&) -- a 0x18-byte record constructor: two flags, two longs and two name strings
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005271C0, length 62 bytes (64 with padding); no direct reference (an inline
// record constructor emitted out of line). Part of the catalogue row 005271b0 (skipped ledger, chunk 904); the class
// label is propagated, so the record is modelled locally.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (landed) for the strings at +0x0c and +0x14.
// Shape: the two byte arguments loaded first (al, dl), `this` parked in esi, members stored in declaration order
// (+0 flag, +4 long, +8 flag, +0x0c string copy, +0x10 long, +0x14 string copy), `mov eax,esi ; ret 18h`.
struct CCharString {                           // 4 bytes: the shared buffer pointer
    void* data;
    CCharString(const CCharString& other);     // 0x0099EC30
};

struct SFilterRecord_Retail {
    bool        m_flag0;                       // +0x00
    long        m_value4;                      // +0x04
    bool        m_flag8;                       // +0x08
    CCharString m_name0C;                      // +0x0c
    long        m_value10;                     // +0x10
    CCharString m_name14;                      // +0x14
    SFilterRecord_Retail(bool flag0, const CCharString& name14, long value10, long value4, bool flag8, const CCharString& name0C);
};

SFilterRecord_Retail::SFilterRecord_Retail(bool flag0, const CCharString& name14, long value10, long value4, bool flag8, const CCharString& name0C)
    : m_flag0(flag0), m_value4(value4), m_flag8(flag8), m_name0C(name0C), m_value10(value10), m_name14(name14)
{
}