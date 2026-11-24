// CInterestingToVillagersDef::CInterestingToVillagersDef() -- the def base (0x0044C0C0), the float at +0x28 set to 1.0 (before the vtable store in this size-optimised module), this class's vtable (0x0123C6D4)
// Name fix (NF387, T3 nf381 0x0044C0C0): the base at 0x0044C0C0 is CSubDefClassBase (retail RTTI CInterestingToVillagersDef : CSubDefClassBase; ??0CSubDefClassBase@@QAE@XZ in the Ego_r and FableWin definition_manager.obj; class key V in both name sets); was the made-up CDefBase_0044c0c0.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D4FCF, length 23 bytes.
// Shape: `fld1 ; fstp`, `mov eax,esi`.
#pragma optimize("s", on)
class CSubDefClassBase { public:                        // the def base (its constructor @ 0x0044C0C0 is labelled "CShoveHaPennyRegionDef", propagated)
    virtual void v00();
    CSubDefClassBase();
};
class CInterestingToVillagersDef : public CSubDefClassBase { public:   // vtable 0x0123C6D4
    unsigned char _pad04[0x24];
    float         Interest;                            // +0x28
    virtual void v00();
    CInterestingToVillagersDef();
};

CInterestingToVillagersDef::CInterestingToVillagersDef()
    : CSubDefClassBase(), Interest(1.0f)
{
}