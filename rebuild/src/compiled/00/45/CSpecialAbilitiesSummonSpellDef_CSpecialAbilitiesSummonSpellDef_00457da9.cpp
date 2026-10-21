// Retry 10: RELOCATION_MATCH -- the vector modelled as STLport _Vector_base (plain _M_start/_M_finish, _M_end_of_storage an _STLP_alloc_proxy with its own ctor); three plain pointers schedule the member-ctor `lea` one store too early.
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
struct CAllocProxy {                            // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own ctor
    T* p;
    CAllocProxy(T* v) : p(v) { }
};
template <class T>
struct CRetailVector {                          // STLport _Vector_base: _M_start / _M_finish / _M_end_of_storage
    T* first;
    T* last;
    CAllocProxy<T> end;
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