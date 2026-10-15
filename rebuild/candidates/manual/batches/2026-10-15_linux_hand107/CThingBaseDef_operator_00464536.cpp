// CThingBaseDef::operator=(const CThingBaseDef& other) -- (propagated label) memberwise def assignment: the base def's out-of-line operator=, then a dword, two 14-byte records and a trailing dword
// Leaf: ??4CThingBaseDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x00464536, 71 bytes (reached through the incremental-link thunk 0x00464531).
// Callees: the base operator= @ 0x00463950 (manifest "CThingBaseDef::operator=").
// Identity: the PDB CThingBaseDef is 0x74 bytes and its operator= is the 0x00463950 callee; this 0x94-byte body is
// the assignment of a def derived from it. The catalog name is kept for the symbol; the derived members are
// modelled with descriptive names.
// Pragma: `#pragma optimize("s", on)` (unaligned start; the speed form copies the records through eax/ecx dwords).
// Shape: the compiler-style memberwise copy: `other` in ebp, `this` in ebx, the dword at +0x70 through eax, the
// two 14-byte two-byte-aligned PODs as `movsd x3 ; movsw` block copies through esi/edi, the dword at +0x90 last.
#pragma optimize("s", on)
struct SDefRecord14 {                                // 14-byte POD (seven shorts)
    unsigned short m_values[7];
};

struct CThingBaseDefBase {
    unsigned char _pad00[0x70];
    CThingBaseDefBase& operator=(const CThingBaseDefBase& other);   // 0x00463950
};

struct CThingBaseDef : CThingBaseDefBase {
    long         m_value70;                          // +0x70
    SDefRecord14 m_record74;                         // +0x74
    SDefRecord14 m_record82;                         // +0x82
    long         m_value90;                          // +0x90
    CThingBaseDef& operator=(const CThingBaseDef& other);
};

CThingBaseDef& CThingBaseDef::operator=(const CThingBaseDef& other)
{
    CThingBaseDefBase::operator=(other);
    m_value70 = other.m_value70;
    m_record74 = other.m_record74;
    m_record82 = other.m_record82;
    m_value90 = other.m_value90;
    return *this;
}
