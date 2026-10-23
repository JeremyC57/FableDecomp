// Grade: DIFFER(873v893) -- frame 0x1c v 0x14: iterator/def and building/hero slots not shared, the 0xD5 shop link homed in memory (retail esi), bool slots -3/-4 swapped; def copy inc/inc/dec matched via CRefCounted base
// CTCVillage::UpdateCurfew() -- step to the next village building and decide whether its doors should be locked for the curfew (occupied homes at curfew time, shops after hours, never with the player inside an awake home), unless something stands in a doorway, then lock or unlock its doors and keep its shop-closed flag in step
// Leaf: ?UpdateCurfew@CTCVillage
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065AD37, 893 bytes, plain `ret`, ebp frame (size TU, `optimize("s",on)`).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCBase;
struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CBaseIntelligentPointer {
    void* _vfptr;
    void* m_pItem;
};
class CThing;
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetPItem() const;                        // 0x00A01B50
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CRefCounted {
    long m_count;                               // +0x04
    void AddRef() { ++m_count; }
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CBuildingDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0xa0 - 0x08];
    unsigned char Flags;                        // +0xa0 bit 2: never locked (descriptive)
};
struct CBuildingMaterial : CDefVirtual, CRefCounted {
    unsigned char _pad08[0x3c - 0x08];
    unsigned char Flags;                        // +0x3c bit 0 (descriptive)
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    __forceinline CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->m_count == 0)
                    Object->Destroy();
            }
            Object = other.Object;
            if (Object)
                Object->AddRef();
        }
        return *this;
    }
    __forceinline ~CDefPointer()
    {
        if (Object) {
            if (--Object->m_count == 0)
                Object->Destroy();
        }
    }
    T* operator->() const { return Object; }
};
template <class T> struct CDefPointerC {
    T* Object;
    __forceinline ~CDefPointerC()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
};
struct CTCShopKeeperLink {                      // interface 0xD5 (descriptive)
    unsigned char _pad00[0x11];
    bool OpenAtNight;                           // +0x11
    bool Closed;                                // +0x12
    unsigned char _pad13[0x38 - 0x13];
    CIntelligentPointer<CThing> Owner;          // +0x38
};
struct CTCShopClosed {                          // interface 0xBD (descriptive)
    unsigned char _pad00[0x0c];
    bool Closed;                                // +0x0c
};
class CThing {
public:
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<CBuildingDef> PDef;             // +0x70
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> __forceinline T* Find(long interfaceType)
    {
        T* p = 0;
        GetInterface(&p, interfaceType);
        return p;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
            return true;
        }
        return false;
    }
    __forceinline CDefPointer<CBuildingDef> GetDef() const { return PDef; }
};
class CThingBuilding : public CThing {
public:
    unsigned char _pad74[0xd0 - 0x74];
    long          NumBlockers;                  // +0xd0 (descriptive)
    unsigned char BuildingFlags;                // +0xd4 bit 0 (descriptive)
    CDefPointerC<CBuildingMaterial> PeekPMaterial();   // 0x0082D2D0
    bool IsUnlocked();                          // 0x0082DEF0
    bool IsDoorBlockedBy(CThing* thing);        // 0x0082D9C0
    void LockAllDoors(bool lock, bool immediately);   // 0x0082DB00
};
bool __fastcall IS_THING_ALIVE(CThing* const& thing);   // 0x0043DBD0
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThing>& thing);   // 0x004AB8F0
struct CPlayer {
    CThing* GetHeroThing();                     // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};

template <class T> struct SNode {
    SNode* _M_next;
    SNode* _M_prev;
    T      _M_data;
};
template <class T> struct CListIt {
    SNode<T>* _M_node;
};
template <class T> struct CRetailList {
    SNode<T>* _M_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        for (SNode<T>* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
};
void advance(CListIt<CIntelligentPointer<CThingBuilding> >& it, long n);   // 0x0065F8B2
struct HasBuildingAsHomeOrWork { CThingBuilding* Building; };
long __stdcall count_if(SNode<CIntelligentPointer<CThing> >* first, SNode<CIntelligentPointer<CThing> >* last, HasBuildingAsHomeOrWork pred);   // 0x006600EC

struct CTCVillage {
    unsigned char _pad00[0x28];
    CRetailList<CIntelligentPointer<CThingBuilding> > Buildings;   // +0x28
    CRetailList<CIntelligentPointer<CThing> > VillagePeople;       // +0x2c
    unsigned char _pad30[0x8c - 0x30];
    unsigned long CurfewBuildingIndex;          // +0x8c
    CPlayerManager* GetPlayerManager();         // 0x00686D40 (CTCBase)
    bool BuildingContainsPlayer(CThingBuilding* building);   // 0x0065AC0E
    bool HomeUsersAreAsleep(CThingBuilding* building);       // 0x0065AC83
    bool IsCurfewTime();                        // 0x00658DB3
    bool BuildingContainsCorrectVillagers(CThingBuilding* building);   // 0x00658EC3
    void UpdateCurfew();
};

void CTCVillage::UpdateCurfew()
{
    if (Buildings.size() == 0)
        return;
    ++CurfewBuildingIndex;
    if (CurfewBuildingIndex >= Buildings.size())
        CurfewBuildingIndex = 0;
    CListIt<CIntelligentPointer<CThingBuilding> > it;
    it._M_node = Buildings._M_node->_M_next;
    advance(it, CurfewBuildingIndex);
    CThingBuilding* building = it._M_node->_M_data.GetPItem();
    CDefPointer<CBuildingDef> def;
    def = building->GetDef();
    if (def.Object && (def->Flags & 4))
        return;
    bool canLock = true;
    CDefPointerC<CBuildingMaterial> material = building->PeekPMaterial();
    if (material.Object && !(material->Flags & 1))
        canLock = false;
    bool playerInside;
    HasBuildingAsHomeOrWork pred = { building };
    bool hasVillagers = count_if(VillagePeople._M_node->_M_next, VillagePeople._M_node, pred) > 0;
    playerInside = BuildingContainsPlayer(building);
    if (playerInside && HomeUsersAreAsleep(building))
        playerInside = false;
    bool lock;
    CTCShopKeeperLink* shop = building->Find<CTCShopKeeperLink>(0xd5);
    if (shop && shop->Owner.GetPItem()) {
        lock = false;
    } else if (hasVillagers) {
        if (building->HasTC(0xab))
            lock = false;
        else if (building->HasTC(0x53) || (IsCurfewTime() && canLock))
            lock = true;
        else
            lock = false;
    } else {
        lock = !(shop && shop->OpenAtNight && !shop->Closed);
    }
    if (building->BuildingFlags & 1) {
        if (!building->HasTC(0x53))
            return;
        if (!building->HasTC(0xbd))
            return;
        if (!((CTCShopClosed*)building->GetTC(0xbd))->Closed)
            return;
        lock = true;
    }
    canLock = BuildingContainsCorrectVillagers(building) && !playerInside;
    if (building->NumBlockers != 0)
        canLock = false;
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
    if (building->IsUnlocked() && lock && canLock) {
        if (IS_THING_ALIVE(hero) && building->IsDoorBlockedBy(hero))
            canLock = false;
        for (SNode<CIntelligentPointer<CThing> >* node = VillagePeople._M_node->_M_next; node != VillagePeople._M_node; node = node->_M_next) {
            if (IS_THING_ALIVE(node->_M_data) && building->IsDoorBlockedBy(node->_M_data.GetPItem())) {
                canLock = false;
                break;
            }
        }
    }
    building->LockAllDoors(lock && canLock, true);
    if (building->HasTC(0xbd)) {
        CTCShopClosed* closed = (CTCShopClosed*)building->GetTC(0xbd);
        bool value;
        if (closed->Closed)
            value = lock;
        else
            value = lock && BuildingContainsCorrectVillagers(building);
        closed->Closed = value && hasVillagers;
    }
}
