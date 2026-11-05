// CTCTavernGame::PlayerHasBeenHit() -- true when the main player's hero is alive and its entity-events component holds a hit-by event this frame
// Leaf: PlayerHasBeenHit
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EB510, length 116 bytes (padded row 128).
// Callees: CTCBase::GetPlayerManager @ 0x00686D40 (`[[0x13B8A1C]+0x30]`), CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature
// @ 0x00487DC0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (inlined GetInterface for CTCEntityEvents, interface
// 0x42 = bit 2 of CThing+0x28), CTCEntityEvents::GetEvent<const NEntityEvents::CEventHitBy, NEntityEvents::CEventFilter_None>(const CEventHitBy**,
// const CEventFilter_None&) @ 0x005ED5A0.
// Layout: CThing dead flag +0x91 bit 0.
// Shape: as the landed CTCCreatureFlyInCircles::CheckForHits (0x007DD990): `IsThingAlive(hero) && hero->GetInterface(&events, 0x42)`; the
// empty filter is a temporary passed by reference (top byte of the first frame dword), the event out pointer reuses the key's slot.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
static inline bool IsThingAlive(const CThing* thing)
{
    return thing && !thing->IsDead();
}
namespace NEntityEvents {
class CEventHitBy;
class CEventFilter_None { public: CEventFilter_None() { } };
}
class CTCEntityEvents { public:
    bool GetEvent(const NEntityEvents::CEventHitBy** out, const NEntityEvents::CEventFilter_None& filter) const;   // 0x005ED5A0
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};
class CTCTavernGame : public CTCBase { public:
    bool PlayerHasBeenHit();
};

bool CTCTavernGame::PlayerHasBeenHit()
{
    CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    const CTCEntityEvents* events;
    if (IsThingAlive(hero) && hero->PeekTC(TCI_ENTITY_EVENTS, &events)) {
        const NEntityEvents::CEventHitBy* event;
        if (events->GetEvent(&event, NEntityEvents::CEventFilter_None()))
            return true;
    }
    return false;
}