// CTCBuyableHouse::Evict() -- pay the money bags to the hero, then for every villager whose home or work is this house: clear that link (no longer game-state persisted), detach it from its shop and from the village
// Leaf: Evict
// Retail 0x006C0FD0, 384 bytes, `ret`.
// Callees: CTCBuyableHouse::DestroyMoneyBags(bool) 0x006C0580, CVectorMap::LowerBound 0x0040F020 (interfaces 0x23, 0x90),
// the 0x23 component's village accessor 0x007EE540, malloc 0x00BFEA0E / free 0x00BFEA14 (the list sentinel),
// copy_if<list iterator, back_insert_iterator<list<CIntelligentPointer<CThingAICreature>>>, HasBuildingAsHomeOrWork>
// 0x006C1ED0 (`__fastcall`, hidden result in ecx), CBaseIntelligentPointer::GetPItem 0x00A01B50, the creature's SetHome
// 0x00512210 / SetWorkplace 0x00512260 (CThing*), CThing::SetAsGameStatePersisted(bool) 0x004C7880,
// CTCShopKeeper::DetachFromShop 0x00799000, CTCVillage::DetachFromDummyVillager(CThing*) 0x0065B2C4, list::clear 0x00510D40.
// Layout: CTCBase ParentThing +4; the village's resident list (sentinel pointer) +0x2C; CThingAICreature home pointer
// +0x168, work pointer +0x170, flag byte +0x92 (bit 4: persisted-by-house); CThing bitset +0x20, map +0x44.
// Shape: the residents are collected into a one-word STLport list local (sentinel malloc'd inline, cleared and freed
// inline at the end) through copy_if with the house predicate (back_insert_iterator has a user ctor, so copy_if returns it
// through a hidden pointer); the 0x23 component is a named local (an unnamed GetTC result moves its key to a second slot);
// the walk caches the end pointer in a frame local.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
class CTCBase;
class CBaseObject;
class CThing;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CBaseIntelligentPointer {
    void*        __vftable;
    CBaseObject* PData;
    CThing* GetPItem() const;                           // 0x00A01B50
};
class CThing { public:
    unsigned char m_pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    void SetAsGameStatePersisted(bool persisted);       // 0x004C7880
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        return (T*)((it == map->last || type < it->first) ? map->last : it)->second;
    }
};
class CThingAICreature : public CThing { public:
    unsigned char           _pad4c[0x92 - 0x4c];
    unsigned char           m_flags92;                  // +0x92 (bit 4)
    unsigned char           _pad93[0x168 - 0x93];
    CBaseIntelligentPointer Home;                       // +0x168
    CBaseIntelligentPointer Workplace;                  // +0x170
    void SetHome(CThing* home);                         // 0x00512210
    void SetWorkplace(CThing* work);                    // 0x00512260
};
struct SCreatureNode {
    SCreatureNode*          _M_next;
    SCreatureNode*          _M_prev;
    CBaseIntelligentPointer _M_data;                    // +0x08
};
struct CCreatureListIterator { SCreatureNode* _M_node; };
struct CCreatureList {                                  // STLport list<CIntelligentPointer<CThingAICreature>> (sentinel pointer)
    SCreatureNode* _M_node;
    CCreatureList() : _M_node(0)
    {
        SCreatureNode* node = (SCreatureNode*)malloc(sizeof(SCreatureNode));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
    ~CCreatureList()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
    void clear();                                       // 0x00510D40
    CCreatureListIterator begin() const { CCreatureListIterator it; it._M_node = _M_node->_M_next; return it; }
    CCreatureListIterator end() const { CCreatureListIterator it; it._M_node = _M_node; return it; }
};
struct CBackInserter {                                  // back_insert_iterator: a user ctor makes it a non-POD return
    CCreatureList* Container;
    explicit CBackInserter(CCreatureList& list) : Container(&list) { }
};
inline CBackInserter back_inserter(CCreatureList& list) { return CBackInserter(list); }
class HasBuildingAsHomeOrWork { public:
    CThing* Building;
};
CBackInserter __fastcall copy_if(CCreatureListIterator first, CCreatureListIterator last, CBackInserter out,
                                 HasBuildingAsHomeOrWork pred);   // 0x006C1ED0
class CTCVillage { public:
    unsigned char _pad00[0x2c];
    CCreatureList Residents;                            // +0x2c
    void DetachFromDummyVillager(CThing* villager);     // 0x0065B2C4
};
class CTCVillageMember { public:
    CTCVillage* GetVillage();                           // 0x007EE540
};
class CTCShopKeeper { public:
    void DetachFromShop();                              // 0x00799000
};
class CTCBuyableHouse {
public:
    void*   __vftable;                                  // +0x00
    CThing* ParentThing;                                // +0x04
    void DestroyMoneyBags(bool giveToHero);             // 0x006C0580
    void Evict();
};

void CTCBuyableHouse::Evict()
{
    DestroyMoneyBags(true);
    CThing* house = ParentThing;
    if (house->HasTC(0x23)) {
        CTCVillageMember* member = house->GetTC<CTCVillageMember>(0x23);
        CTCVillage* village = member->GetVillage();
        if (village) {
            CCreatureList residents;
            HasBuildingAsHomeOrWork pred;
            pred.Building = house;
            copy_if(village->Residents.begin(), village->Residents.end(), back_inserter(residents), pred);
            SCreatureNode* end = residents._M_node;
            for (SCreatureNode* node = end->_M_next; node != end; node = node->_M_next) {
                CThingAICreature* creature = (CThingAICreature*)node->_M_data.GetPItem();
                if (creature) {
                    if (creature->Home.GetPItem() == house) {
                        creature->SetHome(0);
                        creature->SetAsGameStatePersisted(false);
                        creature->m_flags92 &= ~0x10;
                    }
                    if (creature->Workplace.GetPItem() == house) {
                        creature->SetWorkplace(0);
                        creature->SetAsGameStatePersisted(false);
                        creature->m_flags92 &= ~0x10;
                    }
                    if (creature->HasTC(0x90))
                        creature->GetTC<CTCShopKeeper>(0x90)->DetachFromShop();
                    village->DetachFromDummyVillager(creature);
                }
            }
        }
    }
}