// CTCCrateStack::GetNumCrates() const -- 2 when the second crate slot holds a crate, else 1 or 0 for the first slot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B6660, length 38 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice).
// Layout = Ego_r.pdb CTCCrateStack (0x20 bytes): FirstCrate +0xc, SecondCrate +0x14 (CIntelligentPointer<CThing>),
// CrateDefIndex +0x1c.
// Shape: the second slot is tested first with its own `mov eax,2` return; the first slot's pointer becomes a
// `long` 0/1 through `neg ; sbb ; neg` (`!= 0`).
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
    long GetNumCrates() const;
};

long CTCCrateStack::GetNumCrates() const
{
    if (SecondCrate.GetPItem())
        return 2;
    return FirstCrate.GetPItem() != 0;
}