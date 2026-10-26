// CTCClock::FrameUpdate() -- advance the clock hands' animation by the time-of-day change since the last update and remember the current time of day
//
// Retail 0x00732950, 133 bytes (plain `ret`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020, CTCAnimationComplex::GetAnimation @ 0x0070B460 (manifest "CWorld::DrawGetEnvironment",
// propagated), CTCClock::GetHandAnimationUpdateAmount @ 0x00732730 (landed), CTCBase::GetWorld
// @ 0x00686A70 (manifest "CViewBase::GetDisplayEngine", propagated), CAnimateThing::UpdateFrameLogic
// @ 0x0070E8B0.
// Layout: Ego_r.pdb CTCClock (0x14): PDef +0x0c, LastTimeUpdated +0x10; the world's +0x1c record holds
// the normalised time of day at +8 (landed GetHandAnimationUpdateAmount 0x00732730).
// Shape: the animation-complex lookup is not tested, so the absent arm calls GetAnimation through the
// never-written local (it reads the stale key frame word, `mov ecx,[esp+8]`); the call is spelled in
// both arms (hit arm on the lookup expression) so VC7.1 cross-jumps them with the component already in
// ecx -- one call after a joined pointer copies it through eax (+2 bytes). The animation is a named
// local (esi); the 12-byte update
// record {amount, 0, 0} is built from the float result (`fstp`) and the time of day is copied as a
// dword into LastTimeUpdated just before the update call.
class CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};

struct CAnimFrameUpdate {                                   // 12-byte update record
    float Time;                                             // +0x00
    float Reserved0;                                        // +0x04
    float Reserved1;                                        // +0x08
    CAnimFrameUpdate(float time) : Time(time), Reserved0(0.0f), Reserved1(0.0f) {}
};

class CAnimateThing { public:
    void UpdateFrameLogic(const CAnimFrameUpdate& update);  // 0x0070E8B0
};

class CTCAnimationComplex { public:
    CAnimateThing* GetAnimation();                          // 0x0070B460
};

struct CTimeOfDay {
    unsigned char _pad[8];
    float         Time;                                     // +0x08 normalised time of day
};

class CWorld { public:
    unsigned char _pad[0x1c];
    CTimeOfDay*   TimeOfDay;                                // +0x1c
};

class CTCClock { public:
    virtual void v00();
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[4];
    void*         PDef;                                     // +0x0c CDefPointer<const CClockDef>
    float         LastTimeUpdated;                          // +0x10
    CWorld* GetWorld();                                     // 0x00686A70 (CTCBase)
    float GetHandAnimationUpdateAmount();                   // 0x00732730
    void FrameUpdate();
};

void CTCClock::FrameUpdate()
{
    CTCAnimationComplex* complex;
    CAnimateThing* animation;
    if (ParentThing->HasInterface(0x5a))
        animation = ((CTCAnimationComplex*)ParentThing->GetInterface(0x5a))->GetAnimation();
    else
        animation = complex->GetAnimation();
    CAnimFrameUpdate update(GetHandAnimationUpdateAmount());
    LastTimeUpdated = GetWorld()->TimeOfDay->Time;
    animation->UpdateFrameLogic(update);
}