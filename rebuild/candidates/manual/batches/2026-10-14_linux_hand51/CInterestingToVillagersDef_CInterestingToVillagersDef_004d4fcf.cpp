// CInterestingToVillagersDef::CInterestingToVillagersDef() -- the def base (0x0044C0C0), the float at +0x28 set to 1.0 (before the vtable store in this size-optimised module), this class's vtable (0x0123C6D4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4FCF, length 23 bytes.
// Shape: `fld1 ; fstp`, `mov eax,esi`.
#pragma optimize("s", on)
struct CDefBase_0044c0c0 {                        // the def base (its constructor @ 0x0044C0C0 is labelled "CShoveHaPennyRegionDef", propagated)
    virtual void v00();
    CDefBase_0044c0c0();
};
struct CInterestingToVillagersDef : CDefBase_0044c0c0 {   // vtable 0x0123C6D4
    unsigned char _pad04[0x24];
    float         m_28;                            // +0x28
    virtual void v00();
    CInterestingToVillagersDef();
};

CInterestingToVillagersDef::CInterestingToVillagersDef()
    : CDefBase_0044c0c0(), m_28(1.0f)
{
}
