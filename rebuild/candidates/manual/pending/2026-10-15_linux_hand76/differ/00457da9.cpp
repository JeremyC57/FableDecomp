// Grade: DIFFER(ours v retail) -- 58v58, scheduling only: the lane hoists `lea ecx,[esi+64h]` above the three SummonTintColour zero stores, retail places it before the last one (the 24i lea-scheduling lane gap); without the pragma the -1 store becomes a 7-byte mov
// CSpecialAbilitiesSummonSpellDef::CSpecialAbilitiesSummonSpellDef() -- base CParentDefClassBase() (0x00430370), this class's vtable, the TimeOfSummon / SummonTintColour vectors empty, the creature-name def string -1, the two eye dummy names constructed (0x0099E4B0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00457DA9, length 58 bytes.
// Callees: 0x00430370 (the shared def root constructor), 0x0099E4B0 (CCharString::CCharString(), twice).
// Vtable 0x01234674 = CSpecialAbilitiesSummonSpellDef (rtti-vtables.tsv; slot 20 GetSizeofClass 0x00457DE3
// right behind this body). Layout from the PDB (Ego_r, 120 bytes) shifted to the retail build (def root
// ends at +0x3c, 12-byte retail vectors): TimeOfSummon +0x3c, BaseSummonCreatureName +0x48, FadeInTime
// +0x4c, FadeOutTime +0x50, ResetCreatureManaCost +0x54, SummonTintColour +0x58, the eye dummy names +0x64
// and +0x68 (0x6c bytes).
// Shape: vtable store, the first vector's three zero stores, the CDefString's inline constructor as
// `or dword ptr [esi+48h],-1` (the size-optimised def module, `#pragma optimize("s", on)`), the second
// vector, then the two out-of-line CCharString default constructors in declaration order.
#pragma optimize("s", on)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _base[0x38];                  // +0x04..+0x3b: the def root's own members
    CParentDefClassBase();                      // 0x00430370
};

template <class T>
struct CRetailVector {                          // VC6-era Dinkumware vector: first / last / end
    T* first;
    T* last;
    T* end;
    CRetailVector() : first(0), last(0), end(0) { }
};

struct CDefString {                             // a def-name table index, -1 = none
    long m_index;
    CDefString() : m_index(-1) { }
};

class CCharString {
public:
    CCharString();                              // 0x0099E4B0
    void* m_pData;
};

struct CSpecialAbilitiesSummonSpellDef : CParentDefClassBase {
    CRetailVector<float> TimeOfSummon;              // +0x3c
    CDefString           BaseSummonCreatureName;    // +0x48
    float                FadeInTime;                // +0x4c
    float                FadeOutTime;               // +0x50
    long                 ResetCreatureManaCost;     // +0x54
    CRetailVector<long>  SummonTintColour;          // +0x58
    CCharString          EyeLeftDummyPointName;     // +0x64
    CCharString          EyeRightDummyPointName;    // +0x68
    virtual void v00();
    CSpecialAbilitiesSummonSpellDef();
};

CSpecialAbilitiesSummonSpellDef::CSpecialAbilitiesSummonSpellDef()
    : CParentDefClassBase()
{
}
