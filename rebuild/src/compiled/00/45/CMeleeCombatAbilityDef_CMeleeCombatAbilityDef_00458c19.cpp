// Retry 10: RELOCATION_MATCH -- the vector modelled as STLport _Vector_base: _M_start/_M_finish plain pointers and _M_end_of_storage an _STLP_alloc_proxy with its own constructor; with three plain pointer members the `lea ecx,[esi+68h]` for StatsNeeded is scheduled one store too early.
// CMeleeCombatAbilityDef::CMeleeCombatAbilityDef() -- base CParentDefClassBase() (0x00430370), this class's vtable, the two flourish def strings -1, the ValidWeaponClasses vector empty, the StatsNeeded package constructed (0x0045324F)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00458C19, length 45 bytes.
// Leaf: ??0CMeleeCombatAbilityDef
// The chunk label (CSpecialAbilitiesSummonSpellDef) is propagated: the vtable stored here, 0x01234D5C, is
// CMeleeCombatAbilityDef (rtti-vtables.tsv; its slot 20, GetSizeofClass, is 0x00458C46, right behind this
// body). Callees: 0x00430370 (the shared def root constructor), 0x0045324F (the CHeroAbilityStatPackage
// constructor; its manifest label CDisplayManager::CopyBackBufferToTexture is propagated garbage).
// Layout from the PDB (Ego_r, 136 bytes) shifted to the retail build (def root ends at +0x3c, 12-byte
// retail vectors): FlourishAnimName +0x3c, FlourishResponseAnimName +0x40, DamageMultiplier +0x44, the
// Flourish / BlockCounter bytes +0x48/+0x49, AbilityButtonGraphicOverride +0x4c, ValidWeaponClasses +0x50,
// Decapitate +0x5c, KnockDownEffects +0x60, EvadeAllHits / CauseRecoil +0x64/+0x65, StatsNeeded +0x68,
// the combo bounds +0x78/+0x7c.
// Shape: vtable store, the two CDefString inline constructors as `or dword ptr [..],-1` (size-optimised def
// module, `#pragma optimize("s", on)`), the vector's three zero stores, the package's out-of-line constructor.
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

struct CHeroAbilityStatPackage {
    long m_stats[4];                            // 16 bytes (PDB)
    CHeroAbilityStatPackage();                  // 0x0045324F
};

struct CMeleeCombatAbilityDef : CParentDefClassBase {
    CDefString              FlourishAnimName;               // +0x3c
    CDefString              FlourishResponseAnimName;       // +0x40
    float                   DamageMultiplier;               // +0x44
    bool                    Flourish;                       // +0x48
    bool                    BlockCounter;                   // +0x49
    long                    AbilityButtonGraphicOverride;   // +0x4c
    CRetailVector<long>     ValidWeaponClasses;             // +0x50 (vector<EWeaponClass>)
    bool                    Decapitate;                     // +0x5c
    long                    KnockDownEffects;               // +0x60
    bool                    EvadeAllHits;                   // +0x64
    bool                    CauseRecoil;                    // +0x65
    CHeroAbilityStatPackage StatsNeeded;                    // +0x68
    long                    ComboMultiplierLowerBound;      // +0x78
    long                    ComboMultiplierUpperBound;      // +0x7c
    virtual void v00();
    CMeleeCombatAbilityDef();
};

CMeleeCombatAbilityDef::CMeleeCombatAbilityDef()
    : CParentDefClassBase()
{
}