// CTCActionUseBuyHouse::OnActionUse(CThing& user) -- after the base handling, when this object belongs (interface 0x33) to a living house that is buyable (0xD5) and the main player's hero has stats (4), run the main-sign or the dress-sign purchase flow with the hero's money
// Leaf: ?OnActionUse@CTCActionUseBuyHouse
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072ED10, 278 bytes (+8 int3 padding), `ret 4`; CTCActionUseBuyHouse vtable 0x01266A7C
//   slot 28 (the catalog's "vfunc_28"; chunk 160: the vfunc_28 rows are OnActionUse(CThing&)).
// Callees: CTCActionUseBase::OnActionUse(CThing&) 0x007EAB90 (manifest "$E2" is propagated), CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound 0x0040F020, CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCBase::GetPlayerManager 0x00686D40,
//   CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, CTCActionUseBuyHouse::OnActionUseMain
//   (CTCBuyableHouse*, long) 0x0072E940 / OnActionUseDress(CTCBuyableHouse*, long) 0x0072E710.
// Layout: CTCBase {vptr, ParentThing +4}, CTCActionUseBase PActionUseDef +0x0c / Usable +0x10, PDef +0x14
//   (CDefPointer<CBuyHouseDef const>, Main at def +0x25), WaitingForConfirmation +0x18, HouseQuestion +0x1c (Ego_r.pdb
//   matches retail); the 0x33 owned-entity component's owner pointer at +0x0D (pack(1)); CTCHeroStats::Money at retail
//   +0x3C (PDB +0x44, the landed -8 shift).
// Shape: nested ifs; the owned-entity lookup is the out-parameter GetInterface with the owner bound to a reference
//   local (`mov eax,[eax+4] ; lea esi,[eax+0Dh]`; HasTC + GetTC gives `mov esi,[eax+4] ; add esi,0Dh`); the house component is a named local (edi, pushed only on that path); the hero stats come from the
//   ternary GetTC with the Main flag read before its component load; the two purchase calls are an if/else.
// Layout (supersedes any local-model note above): engine/CTCHeroStats.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCHeroStats CTCHeroStatsDonor
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
class CTCBase;
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
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_stateFlags;                 // +0x91 (bit 0: dead)
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(push, 1)
class CTCOwnedEntity { public:                         // interface 0x33
    unsigned char _pad00[0x0d];
    CIntelligentPointer<CThing> POwner;             // +0x0d
};
#pragma pack(pop)
class CTCBuyableHouse;                         // interface 0xD5
class CTCHeroStats : public CTCHeroStatsDonor {
 public:                           // interface 4
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CBuyHouseDef { public:
    unsigned char _pad00[0x25];
    bool Main;                                  // +0x25
};
template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};

class CTCBase
{
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};

class CTCActionUseBase : public CTCBase {
public:
    void*   __vftable;                          // +0x00 (CTCBase)
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    void*   PActionUseDef;                      // +0x0c
    bool    Usable;                             // +0x10
    void OnActionUse(CThing& user);             // 0x007EAB90
};
enum EHouseQuestion { HOUSE_QUESTION_NONE };
class CTCActionUseBuyHouse : public CTCActionUseBase {
public:
    CDefPointer<const CBuyHouseDef> PDef;       // +0x14
    bool           WaitingForConfirmation;      // +0x18
    EHouseQuestion HouseQuestion;               // +0x1c
    void OnActionUseMain(CTCBuyableHouse* house, long money);    // 0x0072E940
    void OnActionUseDress(CTCBuyableHouse* house, long money);   // 0x0072E710
    void OnActionUse(CThing& user);
};

void CTCActionUseBuyHouse::OnActionUse(CThing& user)
{
    CTCActionUseBase::OnActionUse(user);
    CTCOwnedEntity* owned;
    if (ParentThing->GetInterface(&owned, 0x33)) {
        CIntelligentPointer<CThing>& owner = owned->POwner;
        CThing* thing = owner.GetPItem();
        if (thing && !(thing->m_stateFlags & 1)) {
            thing = owner.GetPItem();
            if (thing->HasTC(0xd5)) {
                CTCBuyableHouse* house = thing->GetTC<CTCBuyableHouse>(0xd5);
                CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
                if (hero && hero->HasTC(4)) {
                    CTCHeroStats* stats = hero->GetTC<CTCHeroStats>(4);
                    if (PDef->Main)
                        OnActionUseMain(house, stats->Money);
                    else
                        OnActionUseDress(house, stats->Money);
                }
            }
        }
    }
}