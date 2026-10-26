// Behaviour test for the CTCInventoryQuests constructor (0x0061A420): both bases constructed on the thing, containers empty
// (the map with a self-linked header), the initialised flags/indices set and the uninitialised ones left alone.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
class CThing { public: int id; };
template <class P> struct CAllocProxy {
    P _M_data;
    CAllocProxy(P p) : _M_data(p) { }
};
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T*> _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
struct SRbTreeNodeBase { bool _M_color; SRbTreeNodeBase* _M_parent; SRbTreeNodeBase* _M_left; SRbTreeNodeBase* _M_right; };
namespace NUISystem { class CComponent; }
struct SQuestComponentNode : SRbTreeNodeBase { CThing* first; NUISystem::CComponent* second; };
template <class N> struct CNodeAllocProxy {
    N* _M_data;
    CNodeAllocProxy(N* p) : _M_data(p) { }
    N* allocate(unsigned int n) { return n != 0 ? (N*)std::malloc(n * sizeof(N)) : 0; }
};
struct CQuestComponentTreeBase {
    CNodeAllocProxy<SQuestComponentNode> _M_header;
    CQuestComponentTreeBase() : _M_header(0) { _M_header._M_data = _M_header.allocate(1); }
};
struct SLess { };
struct CQuestComponentMap : CQuestComponentTreeBase {
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
static CThing* g_baseThing = 0; static int g_secondary = 0;
namespace NInventory {
    class CTCInventoryBase { public:
        CTCInventoryBase(CThing& thing) { g_baseThing = &thing; }
        virtual ~CTCInventoryBase() { }
    };
}
namespace NHeroInformationScreens {
    class CBase { public:
        CBase() { ++g_secondary; }
        virtual void v00() { }
    };
}
class CTCInventoryQuests : public NInventory::CTCInventoryBase, public NHeroInformationScreens::CBase { public:
    vector<CEnginePrimitiveHandle> InventoryQuestCardSelectPrimitiveHandles;
    vector<CQuestCardItem> QuestCardItemList;
    bool          IsGuildValid;
    bool          InventoryScreen;
    long          QuestCardCount;
    bool          Teleport;
    bool          QuestCardActivatable;
    bool          EnableQuestCardListInitialise;
    bool          ActiveTestQuestMode;
    long          TestQuestSelectedIndex;
    long          TestQuestTopIndex;
    bool          TestQuestShowAll;
    long          TestQuestListSize;
    long          PreviousScreenType;
    CQuestComponentMap AvailableQuests;
    vector<CIntelligentPointerThing> DisplayQuests;
    float         AccumFadeTime;
    CTCInventoryQuests(CThing& thing);
    virtual ~CTCInventoryQuests() { }
    virtual void v00() { }
};

CTCInventoryQuests::CTCInventoryQuests(CThing& thing)
    : CTCInventoryBase(thing), InventoryScreen(true), Teleport(false), QuestCardActivatable(true), ActiveTestQuestMode(true),
      TestQuestSelectedIndex(0), TestQuestTopIndex(0), TestQuestShowAll(true), PreviousScreenType(-1), AccumFadeTime(0)
{
}

int main()
{
    int bad = 0;
    CThing thing; thing.id = 7;
    void* raw = std::malloc(sizeof(CTCInventoryQuests));
    std::memset(raw, 0x5a, sizeof(CTCInventoryQuests));        // poison: uninitialised members keep it
    CTCInventoryQuests* q = new (raw) CTCInventoryQuests(thing);
    if (g_baseThing != &thing || g_secondary != 1) bad |= 1;
    if (q->InventoryQuestCardSelectPrimitiveHandles._M_start || q->QuestCardItemList._M_end_of_storage._M_data || q->DisplayQuests._M_finish) bad |= 2;
    SQuestComponentNode* h = q->AvailableQuests._M_header._M_data;
    if (!h || h->_M_color || h->_M_parent || h->_M_left != h || h->_M_right != h || q->AvailableQuests._M_node_count) bad |= 4;
    if (!q->InventoryScreen || q->Teleport || !q->QuestCardActivatable || !q->ActiveTestQuestMode || !q->TestQuestShowAll) bad |= 8;
    if (q->TestQuestSelectedIndex || q->TestQuestTopIndex || q->PreviousScreenType != -1 || q->AccumFadeTime != 0.0f) bad |= 16;
    long poison; std::memset(&poison, 0x5a, sizeof(poison));
    if (q->QuestCardCount != poison || q->TestQuestListSize != poison) bad |= 32;
    std::free(h);
    q->~CTCInventoryQuests();
    std::free(raw);
    if (bad) { std::printf("0061a420_TEST FAIL\n"); return 1; }
    std::printf("0061a420_TEST PASS\n");
    return 0;
}