// CTCActionUseBuyHouse::OnActionUse(CThing& user) -- after the base handling, when this object belongs (interface 0x33) to a living house that is buyable (0xD5) and the main player's hero has stats (4), run the main-sign or the dress-sign purchase flow with the hero's money
// Leaf: ?OnActionUse@CTCActionUseBuyHouse
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072ED10, 278 bytes (+8 int3 padding), `ret 4`; CTCActionUseBuyHouse vtable 0x01266A7C
//   slot 28 (the catalog's "vfunc_28"; chunk 160: the vfunc_28 rows are OnActionUse(CThing&)).
// Callees: CTCActionUseBase::OnActionUse(CThing&) 0x007EAB90 (manifest "$E2" is propagated), CVectorMap<ETCInterfaceType,
//   CTCBase*>::LowerBound 0x0040F020, CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCBase::GetPlayerManager 0x00686D40,
//   CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetHeroThing 0x00487DC0, CTCActionUseBuyHouse::OnActionUseMain
//   (CTCBuyableHouse*, long) 0x0072E940 / OnActionUseDress(CTCBuyableHouse*, long) 0x0072E710.
// Layout: CTCBase {vptr, ParentThing +4}, CTCActionUseBase PActionUseDef +0x0c / Usable +0x10, PDef +0x14
//   (CDefPointer<CBuyHouseDef const>, Main at def +0x25), WaitingForConfirmation +0x18, HouseQuestion +0x1c (Ego_r.pdb
//   matches retail); the 0x33 owned-entity component's owner pointer at +0x0D (pack(1)); CTCHeroStats::Money at retail
//   +0x3C (PDB +0x44, the landed -8 shift).
// Shape: nested ifs; the owned-entity lookup is the out-parameter GetInterface with the owner bound to a reference
//   local (`mov eax,[eax+4] ; lea esi,[eax+0Dh]`; HasTC + GetTC gives `mov esi,[eax+4] ; add esi,0Dh`); the house component is a named local (edi, pushed only on that path); the hero stats come from the
//   ternary GetTC with the Main flag read before its component load; the two purchase calls are an if/else.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
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
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {                // CIntelligentPointer<CThing>: 8 bytes
    void* m_vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
#pragma pack(push, 1)
struct CTCOwnedEntity {                         // interface 0x33
    unsigned char _pad00[0x0d];
    CBaseIntelligentPointer POwner;             // +0x0d
};
#pragma pack(pop)
struct CTCBuyableHouse;                         // interface 0xD5
struct CTCHeroStats {                           // interface 4
    unsigned char _pad00[0x3c];
    long Money;                                 // +0x3c
};
struct CPlayer {
    CThing* GetHeroThing();                     // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};
struct CBuyHouseDef {
    unsigned char _pad00[0x25];
    bool Main;                                  // +0x25
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};

class CTCActionUseBase {
public:
    void*   __vftable;                          // +0x00 (CTCBase)
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    void*   PActionUseDef;                      // +0x0c
    bool    Usable;                             // +0x10
    CPlayerManager* GetPlayerManager();         // 0x00686D40 (CTCBase)
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
        CBaseIntelligentPointer& owner = owned->POwner;
        CThing* thing = owner.GetPItem();
        if (thing && !(thing->m_stateFlags & 1)) {
            thing = owner.GetPItem();
            if (thing->HasTC(0xd5)) {
                CTCBuyableHouse* house = thing->GetTC<CTCBuyableHouse>(0xd5);
                CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
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
