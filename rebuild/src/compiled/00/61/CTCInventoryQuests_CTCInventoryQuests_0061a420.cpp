// CTCInventoryQuests::CTCInventoryQuests(CThing& thing) -- the quest-card inventory screen: inventory and hero-information-screen bases, empty primitive/card/display lists and quest map, inventory-screen, activatable, test-mode and show-all flags on, test indices 0, no previous screen (-1)
// Leaf: ??0CTCInventoryQuests
// Retail: Fable.exe (TLC, VC7.1) @ 0x0061A420, 221 bytes (`ret 4`; PDB ??0CTCInventoryQuests@@QAE@AAVCThing@@@Z). The catalog
//   row says "CTCOpinionSource::CTCOpinionSource" (propagated): the stored vtables are CTCInventoryQuests' (0x012585B4 primary,
//   0x012585A0 at +0x130, rtti-vtables.tsv) and the members match the PDB CTCInventoryQuests layout.
// Callees: NInventory::CTCInventoryBase::CTCInventoryBase(CThing&) @ 0x005BFF07, NHeroInformationScreens::CBase::CBase() @ 0x0066E734
//   (the secondary base, as the landed vfunc_0 0x0061A220), malloc @ 0x00BFEA0E (the quest map's 0x18-byte header node).
// Layout: Ego_r.pdb CTCInventoryQuests with the retail CTCInventoryBase at 0x130 (PDB 0x148) and 12-byte STLport containers:
//   InventoryQuestCardSelectPrimitiveHandles +0x134, QuestCardItemList +0x140, IsGuildValid/InventoryScreen +0x14c/+0x14d,
//   QuestCardCount +0x150, Teleport..ActiveTestQuestMode +0x154..+0x157, TestQuestSelectedIndex +0x158, TestQuestTopIndex +0x15c,
//   TestQuestShowAll +0x160, TestQuestListSize +0x164, PreviousScreenType +0x168, AvailableQuests +0x16c, DisplayQuests +0x178,
//   AccumFadeTime +0x184. IsGuildValid, QuestCardCount, EnableQuestCardListInitialise and TestQuestListSize stay uninitialised.
// Shape: the STLport `_Vector_base` / `_Rb_tree_base` + `_Rb_tree` models (as 0x007673C0); every scalar is an initialiser and
//   lands after both vtable stores; the zeros share ebx, the trues one `mov al,1`.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
class CThing;
template <class P> struct CAllocProxy {                 // STLport _STLP_alloc_proxy<P, ...> over an empty allocator
    P _M_data;
    CAllocProxy(P p) : _M_data(p) { }
};
template <class T> struct vector {                      // STLport vector: _Vector_base, 12 bytes
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T*> _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
struct SRbTreeNodeBase {                                // STLport _Rb_tree_node_base
    bool             _M_color;
    SRbTreeNodeBase* _M_parent;
    SRbTreeNodeBase* _M_left;
    SRbTreeNodeBase* _M_right;
};
namespace NUISystem { class CComponent; }
struct SQuestComponentNode : SRbTreeNodeBase {          // 0x18 bytes
    CThing*               first;
    NUISystem::CComponent* second;
};
template <class N> struct CNodeAllocProxy {
    N* _M_data;
    CNodeAllocProxy(N* p) : _M_data(p) { }
    N* allocate(unsigned int n) { return n != 0 ? (N*)malloc(n * sizeof(N)) : 0; }
};
struct CQuestComponentTreeBase {                        // STLport _Rb_tree_base
    CNodeAllocProxy<SQuestComponentNode> _M_header;
    CQuestComponentTreeBase() : _M_header(0) { _M_header._M_data = _M_header.allocate(1); }
};
struct SLess { };
struct CQuestComponentMap : CQuestComponentTreeBase {   // map<CThing*, NUISystem::CComponent*>, 12 bytes
    unsigned long _M_node_count;
    SLess         _M_key_compare;
    CQuestComponentMap() : _M_node_count(0)
    {
        _M_header._M_data->_M_color = false;
        _M_header._M_data->_M_parent = 0;
        _M_header._M_data->_M_left = _M_header._M_data;
        _M_header._M_data->_M_right = _M_header._M_data;
    }
};
class CEnginePrimitiveHandle;
class CQuestCardItem;
struct CIntelligentPointerThing;
namespace NInventory {
    class CTCInventoryBase { public:                           // retail size 0x130
        CTCInventoryBase(CThing& thing);                // 0x005BFF07
        virtual ~CTCInventoryBase();
        unsigned char _pad04[0x130 - 0x04];
    };
}
namespace NHeroInformationScreens {
    class CBase { public:                                      // secondary base at +0x130
        CBase();                                        // 0x0066E734
        virtual void v00();
    };
}
class CTCInventoryQuests : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase { public:   // vtables 0x012585B4 / 0x012585A0
    vector<CEnginePrimitiveHandle> InventoryQuestCardSelectPrimitiveHandles;   // +0x134
    vector<CQuestCardItem> QuestCardItemList;           // +0x140
    bool          IsGuildValid;                         // +0x14c
    bool          InventoryScreen;                      // +0x14d
    long          QuestCardCount;                       // +0x150
    bool          Teleport;                             // +0x154
    bool          QuestCardActivatable;                 // +0x155
    bool          EnableQuestCardListInitialise;        // +0x156
    bool          ActiveTestQuestMode;                  // +0x157
    long          TestQuestSelectedIndex;               // +0x158
    long          TestQuestTopIndex;                    // +0x15c
    bool          TestQuestShowAll;                     // +0x160
    long          TestQuestListSize;                    // +0x164
    long          PreviousScreenType;                   // +0x168 EScreenType
    CQuestComponentMap AvailableQuests;                 // +0x16c
    vector<CIntelligentPointerThing> DisplayQuests;     // +0x178
    float         AccumFadeTime;                        // +0x184
    CTCInventoryQuests(CThing& thing);
    virtual ~CTCInventoryQuests();
    virtual void v00();
};

CTCInventoryQuests::CTCInventoryQuests(CThing& thing)
    : CTCInventoryBase(thing), InventoryScreen(true), Teleport(false), QuestCardActivatable(true), ActiveTestQuestMode(true),
      TestQuestSelectedIndex(0), TestQuestTopIndex(0), TestQuestShowAll(true), PreviousScreenType(-1), AccumFadeTime(0)
{
}