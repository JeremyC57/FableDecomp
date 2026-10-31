// CTCWife::InitWifeAtEndOfMarriageFrescoStartFadeIn() -- as the wedding fresco starts fading in, the wife stops following the hero: she is removed from his CTCFollowed list and from his CTCRegionFollower list when she is on them
// Grade: RELOCATION_MATCH (206 bytes) -- the parent thing read through an inline const accessor (`GetParentThing()`) is what
//   moves its load after the component load `mov esi,[eax+4]` (the parked direct `ParentThing` member read loaded it first).
// Leaf: InitWifeAtEndOfMarriageFrescoStartFadeIn
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CF3B0, 206 bytes (+2 int3), `ret`.
// Callees (Ego_r.pdb spellings; reloc_bindings): CTCBase::GetPlayerManager() 0x00686D40 (protected, CPlayerManager&);
//   CPlayerManager::GetMainPlayer() 0x00449970 (CPlayer&); CPlayer::GetPControlledCreature() const 0x00487DC0 (65 bindings);
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020; CTCFollowed::IsFollower(const CThing*) const 0x006D4D70;
//   CTCFollowed::RemoveFollower(CThing&) 0x006D5180; CTCRegionFollower::IsRegionFollower(const CThing*) const 0x006AEB60;
//   CTCRegionFollower::RemoveFollower(CThing&) 0x006AEBA0.
// Name: Ego_r.pdb ?InitWifeAtEndOfMarriageFrescoStartFadeIn@CTCWife@@QAEXXZ; catalog CTCWife::InitWifeAtEndOfMarriageFrescoStartFadeIn.
// Layout: CTCBase ParentThing +0x04 (engine/CTCBase.h names); retail CThing UsingComponents +0x20, Components +0x44, Dead +0x91 bit 0.
// Model: CTCBase is modelled locally (as the landed 0x0065AC0E does) because engine/CTCBase.h carries no member functions and
//   GetPlayerManager is a protected CTCBase member; the inline parent accessor's spelling is unproven (inline, no symbol).
class CTCBase;
enum ETCInterfaceType { TC_REGION_FOLLOWER = 0x7a, TC_FOLLOWED = 0xbc };
template <class K, class V> class CVectorMap { public:
    struct value_type { K first; V second; };
    value_type* _M_start;
    value_type* _M_finish;
    value_type* LowerBound(const K& key);       // 0x0040F020
    __forceinline V Find(long type)
    {
        value_type* it = LowerBound((K)type);
        if (it == _M_finish || type < it->first)
            it = _M_finish;
        return it->second;
    }
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    CVectorMap<ETCInterfaceType, CTCBase*> Components;   // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                     // +0x91 bit 0
    template<class T> __forceinline bool GetTC(T** out, long type)
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            *out = (T*)Components.Find(type);
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
#include "engine/CPlayerManager.h"           // CPlayer& GetMainPlayer() 0x00449970
class CTCFollowed { public:
    bool IsFollower(const CThing* thing) const; // 0x006D4D70
    void RemoveFollower(CThing& thing);         // 0x006D5180
};
class CTCRegionFollower { public:
    bool IsRegionFollower(const CThing* thing) const;   // 0x006AEB60
    void RemoveFollower(CThing& thing);         // 0x006AEBA0
};
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CThing* GetParentThing() const { return ParentThing; }
protected:
    CPlayerManager& GetPlayerManager();         // 0x00686D40
};
class CTCWife : public CTCBase { public:
    void InitWifeAtEndOfMarriageFrescoStartFadeIn();
};

void CTCWife::InitWifeAtEndOfMarriageFrescoStartFadeIn()
{
    CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (!hero || hero->Dead)
        return;
    CTCFollowed* followed;
    if (hero->GetTC(&followed, TC_FOLLOWED)) {
        if (followed->IsFollower(GetParentThing()))
            followed->RemoveFollower(*GetParentThing());
    }
    CTCRegionFollower* regionFollower;
    if (hero->GetTC(&regionFollower, TC_REGION_FOLLOWER)) {
        if (regionFollower->IsRegionFollower(GetParentThing()))
            regionFollower->RemoveFollower(*GetParentThing());
    }
}