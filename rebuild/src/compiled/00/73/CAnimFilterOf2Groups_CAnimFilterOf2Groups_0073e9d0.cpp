// CAnimFilterOf2Groups::CAnimFilterOf2Groups(const CCharString& groupA, const CCharString& groupB)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0073E9D0, length 48 bytes.
// Callees: CCharString::CCharString() @ 0x0099E4B0 twice (the two group names at +4 and +8),
// then the out-of-line initialiser @ 0x0073E040 (`SetGroups(a, b)`, two arguments re-pushed
// from the parameter slots). Vtable 0x01267578 is CAnimFilterOf2Groups (rtti-vtables.tsv;
// PDB size 20); the base CAnimFilterBase contributes only the vptr (its inline constructor's
// vtable store is dead and dropped). `ret 8`: two stack arguments. The manifest label
// (CAIStateGroup_CreatureUseCoverBase dtor) is propagated garbage.
struct CCharString {
    char* m_pData;
    CCharString();                          // 0x0099E4B0
};

struct CAnimFilterBase {
    virtual void v00();
    CAnimFilterBase() { }
};

struct CAnimFilterOf2Groups : CAnimFilterBase {    // vtable 0x01267578
    CCharString m_groupA;                   // +0x4
    CCharString m_groupB;                   // +0x8
    CAnimFilterOf2Groups(const CCharString& groupA, const CCharString& groupB);
    void SetGroups(const CCharString& groupA, const CCharString& groupB);   // 0x0073E040
};

CAnimFilterOf2Groups::CAnimFilterOf2Groups(const CCharString& groupA, const CCharString& groupB)
    : m_groupA()
    , m_groupB()
{
    SetGroups(groupA, groupB);
}