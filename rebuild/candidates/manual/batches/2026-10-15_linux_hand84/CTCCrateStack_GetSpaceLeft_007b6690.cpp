// CTCCrateStack::GetSpaceLeft() const -- free crate slots: 0 when the second slot is filled, else 1 or 2 depending on the first slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B6690, length 36 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice).
// Layout = Ego_r.pdb CTCCrateStack (0x20 bytes): FirstCrate +0xc, SecondCrate +0x14, CrateDefIndex +0x1c.
// Shape: the filled second slot returns 0 (`xor eax,eax`); `first ? 1 : 2` is the branch-free
// `neg ; sbb eax,eax ; add eax,2`.
struct CBaseObject;

struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item link
    virtual void v00();
    void* m_pItem;                                          // +4
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};

struct CTCCrateStack {
    void*                   __vftable;                      // +0x00
    unsigned char           _pad04[8];                      // +0x04 CTCBase
    CBaseIntelligentPointer FirstCrate;                     // +0x0c CIntelligentPointer<CThing>
    CBaseIntelligentPointer SecondCrate;                    // +0x14 CIntelligentPointer<CThing>
    long                    CrateDefIndex;                  // +0x1c
    long GetSpaceLeft() const;
};

long CTCCrateStack::GetSpaceLeft() const
{
    if (SecondCrate.GetPItem())
        return 0;
    return FirstCrate.GetPItem() ? 1 : 2;
}
