// Retry 10: RELOCATION_MATCH -- the vector modelled as STLport _Vector_base (plain _M_start/_M_finish, _M_end_of_storage an _STLP_alloc_proxy with its own ctor); three plain pointers schedule the member-ctor `lea` one store too early.
// CQuestCardDef::CQuestCardDef() -- base def constructor 0x0044C0C0, this class's vtable, the two region-name def strings -1, the RewardObjects vector empty, the Prerequisites map constructed (0x004DF7D3)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E00BC, length 45 bytes.
// Leaf: ??0CQuestCardDef
// The chunk label (CSpecialAbilitiesSummonSpellDef) is propagated: the vtable stored here, 0x01241E44, is
// CQuestCardDef (rtti-vtables.tsv; its slot 20, GetSizeofClass, is 0x004E00E9, right behind this body).
// Callees: 0x0044C0C0 (the base def constructor; manifest label CShoveHaPennyRegionDef is a hint only),
// 0x004DF7D3 (the Prerequisites map constructor; its label CopyBackBufferToTexture is propagated garbage).
// Layout from the PDB (Ego_r, 124 bytes; the base ends at +0x28 in both builds, a retail vector is 12
// bytes): the text ids +0x28..+0x34, RegionName +0x38, TeleporterRegionName +0x3c, InventoryCategory /
// RenownReward / GoldReward +0x40..+0x48, RewardObjects +0x4c, four flags +0x58..+0x5b, Prerequisites +0x5c.
// Shape: vtable store, the two CDefString inline constructors as `or dword ptr [..],-1` (size-optimised
// module, `#pragma optimize("s", on)`), the vector's three zero stores, the map's out-of-line constructor.
#pragma optimize("s", on)
struct CDefBase_0044c0c0 {
    virtual void v00();
    unsigned char _base[0x24];                  // +0x04..+0x27
    CDefBase_0044c0c0();                        // 0x0044C0C0
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

class CDefString { public:                             // a def-name table index, -1 = none
    long m_index;
    CDefString() : m_index(-1) { }
};

struct CPrerequisiteMap {                       // map<EQuestCardPrerequisite, long>
    long m_words[4];
    CPrerequisiteMap();                         // 0x004DF7D3
};

class CQuestCardDef : public CDefBase_0044c0c0 { public:
    long                QuestName;                          // +0x28
    long                QuestSummary;                       // +0x2c
    long                QuestObjective;                     // +0x30
    long                SuccessSummary;                     // +0x34
    CDefString          RegionName;                         // +0x38
    CDefString          TeleporterRegionName;               // +0x3c
    long                InventoryCategory;                  // +0x40
    long                RenownReward;                       // +0x44
    long                GoldReward;                         // +0x48
    CRetailVector<long> RewardObjects;                      // +0x4c
    bool                IsCore;                             // +0x58
    bool                IsVignette;                         // +0x59
    bool                IsExclusive;                        // +0x5a
    bool                MakeVignetteRouteAppearOnMinimap;   // +0x5b
    CPrerequisiteMap    Prerequisites;                      // +0x5c
    virtual void v00();
    CQuestCardDef();
};

CQuestCardDef::CQuestCardDef()
    : CDefBase_0044c0c0()
{
}