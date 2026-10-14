// CRumbleDef::CRumbleDef() -- the CShoveHaPennyRegionDef-style base constructor, this class's vtable, then the two members at +0x28 and +0x34 constructed out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E1722, length 34 bytes.
// Callees: the def base constructor @ 0x0044C0C0 (labelled CShoveHaPennyRegionDef::CShoveHaPennyRegionDef, propagated), the member constructors @ 0x004DFAC5 (+0x28) and 0x004DFAE3 (+0x34) (both labelled "CDisplayManager::CopyBackBufferToTexture", propagated). Vtable 0x0124273C = CRumbleDef (rtti-vtables.tsv; the chunk label "CFireheartMinigameDef" is propagated). Shape: the size-optimised module, the first member address formed before the vtable store; `this` is returned.
#pragma optimize("s", on)
struct CDefMember_004dfac5 { unsigned char _body[12]; CDefMember_004dfac5(); };   // 0x004DFAC5
struct CDefMember_004dfae3 { unsigned char _body[4];  CDefMember_004dfae3(); };   // 0x004DFAE3
struct CShoveHaPennyRegionDef_Base {               // the def base whose constructor is 0x0044C0C0 (the manifest label names a sibling class)
    virtual void v00();
    unsigned char _pad[0x24];                      // +0x04 .. +0x28
    CShoveHaPennyRegionDef_Base();                 // 0x0044C0C0
};

struct CRumbleDef : CShoveHaPennyRegionDef_Base {
    CDefMember_004dfac5    m_28;                 // +0x28 (12 bytes; constructor 0x004DFAC5)
    CDefMember_004dfae3    m_34;                 // +0x34 (constructor 0x004DFAE3)
    virtual void v00();
    CRumbleDef();
};

CRumbleDef::CRumbleDef()
    : CShoveHaPennyRegionDef_Base()
{
}
