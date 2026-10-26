// CTCTavernGame::PlayerHasBeenHit() -- true when the main player's hero is alive and its entity-events component holds a hit-by event this frame
// Leaf: PlayerHasBeenHit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EB510, length 116 bytes (padded row 128).
// Callees: CTCBase::GetPlayerManager @ 0x00686D40 (`[[0x13B8A1C]+0x30]`), CPlayerManager::GetPlayer @ 0x00449970, CPlayer::GetHeroThing
// @ 0x00487DC0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (inlined GetInterface for CTCEntityEvents, interface
// 0x42 = bit 2 of CThing+0x28), CTCEntityEvents::GetEvent<const NEntityEvents::CEventHitBy, NEntityEvents::CEventFilter_None>(const CEventHitBy**,
// const CEventFilter_None&) @ 0x005ED5A0.
// Layout: CThing dead flag +0x91 bit 0.
// Shape: as the landed CTCCreatureFlyInCircles::CheckForHits (0x007DD990): `IsThingAlive(hero) && hero->GetInterface(&events, 0x42)`; the
// empty filter is a temporary passed by reference (top byte of the first frame dword), the event out pointer reuses the key's slot.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
static inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_flags91 & 1);
}
namespace NEntityEvents {
class CEventHitBy;
class CEventFilter_None { public: CEventFilter_None() { } };
}
class CTCEntityEvents { public:
    bool GetEvent(const NEntityEvents::CEventHitBy** out, const NEntityEvents::CEventFilter_None& filter);   // 0x005ED5A0
};
class CPlayer { public:
    CThing* GetHeroThing();                     // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetPlayer();                       // 0x00449970
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    CPlayerManager* GetPlayerManager();         // 0x00686D40
};
class CTCTavernGame : public CTCBase { public:
    bool PlayerHasBeenHit();
};

bool CTCTavernGame::PlayerHasBeenHit()
{
    CThing* hero = GetPlayerManager()->GetPlayer()->GetHeroThing();
    CTCEntityEvents* events;
    if (IsThingAlive(hero) && hero->GetInterface(&events, 0x42)) {
        const NEntityEvents::CEventHitBy* event;
        if (events->GetEvent(&event, NEntityEvents::CEventFilter_None()))
            return true;
    }
    return false;
}