// CTCBuyableHouse::GetCurrentCollectableRentValue() const -- the rent waiting in the house: the gold of every living small gold bag ("OBJECT_GOLDBAG_SMALL_WITH_COINS") among the building's contents
//   name and the 0xCC key (total +0, shared +4, gold index +8); ours gives the 0x21 key its own word (0x10 frame), so
//   every frame offset after it is 4 higher
// Leaf: GetCurrentCollectableRentValue
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local stand-in map class (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 004365b0: ?LowerBound@CInterfaceMap@@QBEPBUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C0160, 271 bytes (+1 int3 padding), `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 (both lookups), CCharString(const char*,
//   long) 0x0099EBF0, ~CCharString 0x0099EAE0, CTCBase::GetDefinitionManager() const 0x006869D0, CDefinitionManager::
//   GetDefGlobalIndexFromName(const CCharString&) 0x009AD410, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CTCGold::GetGold 0x007CBDC0. Literal 0x0125D760.
// Layout: engine/CTCBuyableHouse.h is the PDB view of the class (only ParentThing +4 is read here, so `this` is the
//   CTCBase prefix); the building contents component (interface 0x21) keeps a VC6 list of CIntelligentPointer<CThing>
//   with its head at +0x0c (landed CAIStateGroup_SitForMeal::GetPFreeChair 0x00945B10); CThing dead bit +0x91 bit 0,
//   global def index (unsigned short) +0x8c; interface 0xCC is CTCGold.
// Shape: the total is declared first and zero-initialised (the early returns reuse the zero register, the loop sum
//   stays in its frame word); the contents component is null-tested; the gold bag's def name is a named CCharString in
//   its own block (homed in the lookup key's word); both lookups are const (`const CThing*`), the gold lookup the
//   ternary entry pick with its own null test.
// Tried: out-parameter GetInterface for the contents, an implicit CCharString conversion temporary instead of the named
//   block local, the contents through a `__forceinline` accessor: the 0x21 key never shares the name's word.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];            // +0x20
    CInterfaceMap  m_interfaces;                // +0x44
    unsigned char  _pad54[0x38];
    unsigned short DefIndex;                    // +0x8c global definition index
    unsigned char  _pad8e[0x91 - 0x8e];
    unsigned char  m_stateFlags;                // +0x91 (bit 0: dead)
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
        return (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {                // CIntelligentPointer<CThing>: vptr + item
    void*   __vftable;
    CThing* m_pItem;
    const CThing* GetPItem() const;             // 0x00A01B50
};
struct SThingListNode {
    SThingListNode*         next;
    SThingListNode*         prev;
    CBaseIntelligentPointer value;              // +0x08
};
struct CTCBuildingContents {                    // interface 0x21
    unsigned char   _pad00[0x0c];
    SThingListNode* ThingsHead;                 // +0x0c (VC6 list head)
};
class CTCGold { public:                                // interface 0xCC
    long GetGold() const;                             // 0x007CBDC0
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager
{
};

class CTCBase
{
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

class CTCBuyableHouse : public CTCBase {
public:
    void*         __vftable;                    // +0x00 (CTCBase)
    const CThing* ParentThing;                  // +0x04
    long GetCurrentCollectableRentValue() const;
};

long CTCBuyableHouse::GetCurrentCollectableRentValue() const
{
    long total = 0;
    if (ParentThing->HasTC(0x21)) {
        CTCBuildingContents* contents = ParentThing->GetTC<CTCBuildingContents>(0x21);
        if (contents) {
            long goldIndex;
            {
                CCharString name("OBJECT_GOLDBAG_SMALL_WITH_COINS", -1);
                goldIndex = GetDefinitionManager().GetDefGlobalIndexFromName(name);
            }
            for (SThingListNode* node = contents->ThingsHead->next; node != contents->ThingsHead; node = node->next) {
                const CThing* thing = node->value.GetPItem();
                if (thing && !(thing->m_stateFlags & 1) && thing->DefIndex == goldIndex && thing->HasTC(0xcc)) {
                    CTCGold* gold = thing->GetTC<CTCGold>(0xcc);
                    if (gold)
                        total += gold->GetGold();
                }
            }
        }
    }
    return total;
}