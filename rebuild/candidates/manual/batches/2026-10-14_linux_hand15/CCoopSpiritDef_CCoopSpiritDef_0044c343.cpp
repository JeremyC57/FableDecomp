// CCoopSpiritDef::CCoopSpiritDef() -- base CShoveHaPennyRegionDef(), this vtable, four CDefString members set to -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0044C343, length 33 bytes.
// Callees: CShoveHaPennyRegionDef::CShoveHaPennyRegionDef @ 0x0044C0C0 (the manifest's label; a
// def base shared by the whole 0x0044xxxx def module).
// Vtable 0x01232214 = CCoopSpiritDef (rtti-vtables.tsv); the manifest label "CBedDef" is a
// propagated name (the byte-identical CBedDef constructor is @ 0x004D7A25, vtable 0x0123E8BC).
// Shape: the vtable store precedes the four `-1` stores because each is a member object with an
// inline constructor (a CDefString, "no name" = -1); one `or eax,-1` register feeds all four.
#pragma optimize("s", on)
struct CShoveHaPennyRegionDef {
    virtual void v00();
    unsigned char _body[0x24];                  // +0x04..+0x27
    CShoveHaPennyRegionDef();                   // 0x0044C0C0
};

struct CDefString {                             // a name index into the def string table
    long m_index;
    CDefString() : m_index(-1) { }
};

struct CCoopSpiritDef : CShoveHaPennyRegionDef {   // vtable 0x01232214
    CDefString m_name0;                         // +0x28
    CDefString m_name1;                         // +0x2c
    CDefString m_name2;                         // +0x30
    CDefString m_name3;                         // +0x34
    virtual void v00();
    CCoopSpiritDef();
};

CCoopSpiritDef::CCoopSpiritDef()
    : CShoveHaPennyRegionDef(), m_name0(), m_name1(), m_name2(), m_name3()
{
}
