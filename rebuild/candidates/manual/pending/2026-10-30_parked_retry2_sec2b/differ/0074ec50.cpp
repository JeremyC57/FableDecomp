// CTCContainerRewardHero::CreateItemPickupContainer() -- when the container dies with rewards: spawn a reward chest
// Grade: DIFFER(574v564) 46 difflines (was 74): the empty script names passed as "" literals to the const CCharString& parameter (temporaries built before the pushes, as 0x007805D0); left: empty() compare register form, the vector-dtor start/finish caching, ebp push shrink-wrap (retry wave 2 sec2b)
// Was: DIFFER(572v564) -- empty() compare operand order, ebp push shrink-wrapped, the vector dtor caches start/finish differently
// holding the reward items, and a small gold bag holding the reward gold
// Leaf: CreateItemPickupContainer
//
// Retail 0x0074EC50, 564 bytes (plain `ret`).
// Callees: CTCContainerRewardHero::GetRewardItems(vector<pair<long,CCharString>>&, long&) @ 0x0074E860 / CreateNewObject
// @ 0x0074E560, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CTCBase::GetDefinitionManager
// @ 0x006869D0, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, CThing::GetPosition @ 0x004C73D0,
// CVectorMap::LowerBound @ 0x0040F020 (interfaces 0x3c CTCContainer, 0xcc gold), CTCContainer::InsertItem @ 0x008ED1F0,
// free @ 0x00BFEA14. Strings "OBJECT_CHEST_REWARD_ON_DEATH" 0x012694D4, "" 0x0122D70E, "OBJECT_GOLDBAG_SMALL_WITH_COINS"
// 0x0125D760.
// Layout: CTCBase ParentThing +4; CThing bitset +0x20, map +0x44; the gold component's amount +0x0c.
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
extern "C" void __cdecl free(void*);
struct SContainerItem {                                      // pair<long, CCharString>
    long        first;
    CCharString second;
};
struct CContainerItemVector {                                // STLport vector<pair<long, CCharString>>
    SContainerItem* _M_start;
    SContainerItem* _M_finish;
    SContainerItem* _M_end_of_storage;
    CContainerItemVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CContainerItemVector()
    {
        for (SContainerItem* p = _M_start; p != _M_finish; ++p)
            p->~SContainerItem();
        if (_M_start)
            free(_M_start);
    }
    bool empty() const { return _M_start == _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
    SContainerItem& operator[](unsigned int n) { return _M_start[n]; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
struct CThing {
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];                         // +0x20
    CInterfaceMap  m_interfaces;                             // +0x44
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CThingObject : CThing { };
struct CTCContainer { void InsertItem(long defIndex, const CCharString& name); };   // 0x008ED1F0
struct CTCGold {
    unsigned char _pad00[0x0c];
    long Amount;                                             // +0x0c
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name); // 0x009AD410
};
struct CTCContainerRewardHero {
    virtual void v00();
    CThing* ParentThing;                                     // +0x04 (CTCBase)
    CDefinitionManager* GetDefinitionManager() const;        // 0x006869D0 (CTCBase)
    void GetRewardItems(CContainerItemVector& items, long& gold);   // 0x0074E860
    CThingObject* CreateNewObject(long defIndex, const C3DVector& pos, const C3DVector& velocity,
                                  const CCharString& name);  // 0x0074E560
    void CreateItemPickupContainer();
};

void CTCContainerRewardHero::CreateItemPickupContainer()
{
    long gold = 0;
    CContainerItemVector items;
    GetRewardItems(items, gold);
    if (!items.empty()) {
        long def = GetDefinitionManager()->GetDefGlobalIndexFromName(CCharString("OBJECT_CHEST_REWARD_ON_DEATH"));
        if (def > 0) {
            CThingObject* chest = CreateNewObject(def, ParentThing->GetPosition(), C3DVector(0.0f, 0.25f, 0.25f), "");
            if (chest) {
                CTCContainer* container;
                if (chest->HasTC(0x3c))
                    container = chest->GetTC<CTCContainer>(0x3c);
                if (container) {
                    unsigned int count = items.size();
                    for (unsigned int i = 0; i < count; ++i)
                        container->InsertItem(items[i].first, items[i].second);
                }
            }
        }
    }
    long amount = gold;
    if (amount > 0) {
        long def = GetDefinitionManager()->GetDefGlobalIndexFromName(CCharString("OBJECT_GOLDBAG_SMALL_WITH_COINS"));
        if (def > 0) {
            CThingObject* bag = CreateNewObject(def, ParentThing->GetPosition(), C3DVector(0.0f, 0.25f, 0.25f), "");
            if (bag) {
                CTCGold* goldTC;
                if (bag->HasTC(0xcc))
                    goldTC = bag->GetTC<CTCGold>(0xcc);
                if (goldTC)
                    goldTC->Amount = amount;
            }
        }
    }
}
