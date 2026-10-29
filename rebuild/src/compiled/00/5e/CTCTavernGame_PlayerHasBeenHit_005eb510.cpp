// CTCTavernGame::PlayerHasBeenHit() -- true when the main player's hero is alive and its entity-events component holds a hit-by event this frame
// Leaf: PlayerHasBeenHit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EB510, length 116 bytes (padded row 128).
// Callees: CTCBase::GetPlayerManager @ 0x00686D40 (`[[0x13B8A1C]+0x30]`), CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature
// @ 0x00487DC0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0 (inlined GetInterface for CTCEntityEvents, interface
// 0x42 = bit 2 of CThing+0x28), CTCEntityEvents::GetEvent<const NEntityEvents::CEventHitBy, NEntityEvents::CEventFilter_None>(const CEventHitBy**,
// const CEventFilter_None&) @ 0x005ED5A0.
// Layout: CThing dead flag +0x91 bit 0.
// Shape: as the landed CTCCreatureFlyInCircles::CheckForHits (0x007DD990): `IsThingAlive(hero) && hero->GetInterface(&events, 0x42)`; the
// empty filter is a temporary passed by reference (top byte of the first frame dword), the event out pointer reuses the key's slot.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
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
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
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
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CPlayerManager { public:
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
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
    CTCEntityEvents* events;
    if (IsThingAlive(hero) && hero->GetInterface(&events, 0x42)) {
        const NEntityEvents::CEventHitBy* event;
        if (events->GetEvent(&event, NEntityEvents::CEventFilter_None()))
            return true;
    }
    return false;
}