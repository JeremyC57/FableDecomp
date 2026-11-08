// CTCCreatureFlyInCircles::CheckForHits() -- until it has been hit, look for a hit-by event on the flying creature caused by the hero's shot and remember it in HasBeenHit
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound).
//
// Retail 0x007DD990, 154 bytes (plain `ret`). Callees: CTCBase::PeekWorld() const @ 0x00686A80
// (`[0x013B8A1C]+0x24`; manifest "CViewBase::GetDisplayEngine", propagated), CWorld::PeekMyPlayer
// @ 0x0049D9D0, CPlayer::GetPControlledCreature @ 0x00487DC0 (labels propagated), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x004365B0, CTCEntityEvents::GetEvent<NEntityEvents::CEventHitBy,
// CFilter_Bind2<CIsHitEventHitBy, CIsHitEventHitByShot>>(CEventHitBy**, const filter&) @ 0x007DE2E0.
// Layout: Ego_r.pdb CTCCreatureFlyInCircles: Creature +0x0c, HasBeenHit +0x43.
// Shape: `IsThingAlive(hero) && Creature->GetInterface(&events, 0x42)` (interface 0x42: byte test of
// bitset word 2, `test byte [eax+28h],4`; component in eax); the event pointer is an uninitialised
// local; the filter is a temporary CFilter_Bind2 holding references to two temporaries: the
// CIsHitEventHitBy(hero) (built in the lookup key's frame word, its store scheduled after the pushes)
// and the empty CIsHitEventHitByShot (user ctor, one frame byte).
#include "fable_compiler.h"
class CTCBase;
class CThing;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char m_stateFlags;                             // +0x91 (bit 0 = dead)
};
class CThingPlayerCreature : public CThing { };

inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}

namespace NEntityEvents {
    class CEventHitBy;
    class CIsHitEventHitBy { public:
        const CThing* HitBy;
        CIsHitEventHitBy(const CThing* hitBy) : HitBy(hitBy) {}
    };
    class CIsHitEventHitByShot { public:
        CIsHitEventHitByShot() {}
    };
}

template <class A, class B>
class CFilter_Bind2 { public:
    const A& First;
    const B& Second;
    CFilter_Bind2(const A& first, const B& second) : First(first), Second(second) {}
};

typedef CFilter_Bind2<NEntityEvents::CIsHitEventHitBy, NEntityEvents::CIsHitEventHitByShot> CHitByShotFilter;

class CTCEntityEvents { public:
    bool GetEvent(NEntityEvents::CEventHitBy** out, const CHitByShotFilter& filter);   // 0x007DE2E0
};

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                                 // 0x00487DC0
};

class CWorld {
public:
    CPlayer const* PeekMyPlayer() const;   // 0x0049D9D0 (Ego_r.pdb ?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ)
private:                       // 0x0049D9D0
};

class CTCBase { public:
    virtual ~CTCBase();                                           // slot 0 (0x00686830)
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80 (Ego_r.pdb CTCBase, protected)
};
struct CTCCreatureFlyInCircles : public CTCBase {
    void*               ParentThing;                        // +0x04 (CTCBase)
    unsigned char       _pad08[4];
    CThingCreatureBase* Creature;                           // +0x0c
    unsigned char       _pad10[0x43 - 0x10];
    bool                HasBeenHit;                         // +0x43
    void CheckForHits();
};

void CTCCreatureFlyInCircles::CheckForHits()
{
    if (!HasBeenHit)
    {
        CThing* hero = ((CPlayer*)((CWorld*)&(PeekWorld()))->PeekMyPlayer())->GetPControlledCreature();
        CTCEntityEvents* events;
        if (IsThingAlive(hero) && Creature->GetInterface(&events, 0x42))
        {
            NEntityEvents::CEventHitBy* event;
            if (events->GetEvent(&event, CHitByShotFilter(NEntityEvents::CIsHitEventHitBy(hero), NEntityEvents::CIsHitEventHitByShot())))
                HasBeenHit = true;
        }
    }
}