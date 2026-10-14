// CFireheartMinigameDef::CFireheartMinigameDef() -- the CShoveHaPennyRegionDef-style base constructor, this class's vtable, then the two CCharString members at +0x28 and +0x2c default-constructed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D6638, length 34 bytes.
// Callees: the def base constructor @ 0x0044C0C0 (labelled CShoveHaPennyRegionDef::CShoveHaPennyRegionDef, propagated), CCharString::CCharString @ 0x0099E4B0 twice (+0x28, +0x2c). Vtable 0x0123E584 = CFireheartMinigameDef (rtti-vtables.tsv). Shape: the size-optimised module, the first member address formed before the vtable store; `this` is returned.
#pragma optimize("s", on)
struct CCharString {
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
};
struct CShoveHaPennyRegionDef_Base {               // the def base whose constructor is 0x0044C0C0 (the manifest label names a sibling class)
    virtual void v00();
    unsigned char _pad[0x24];                      // +0x04 .. +0x28
    CShoveHaPennyRegionDef_Base();                 // 0x0044C0C0
};

struct CFireheartMinigameDef : CShoveHaPennyRegionDef_Base {
    CCharString            m_name;                        // +0x28
    CCharString            m_text;                        // +0x2c
    virtual void v00();
    CFireheartMinigameDef();
};

CFireheartMinigameDef::CFireheartMinigameDef()
    : CShoveHaPennyRegionDef_Base()
{
}
