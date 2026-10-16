// CTCClock::FrameUpdate() -- advance the clock hands' animation by the time-of-day change since the last update and remember the current time of day
// Grade: DIFFER(135v133) -- only the component join: retail loads the CTCAnimationComplex straight into ecx on both paths (`mov ecx,[eax+4]` / stale `mov ecx,[esp+8]`), ours into eax plus `mov ecx,eax` before GetAnimation (out-parameter template, CTCBase** out-parameter and an unset-local accessor all give the eax join; float zero members fixed the record stores)
//
// Retail 0x00732950, 133 bytes (plain `ret`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020, CTCAnimationComplex::GetAnimation @ 0x0070B460 (manifest "CWorld::DrawGetEnvironment",
// propagated), CTCClock::GetHandAnimationUpdateAmount @ 0x00732730 (landed), CTCBase::GetWorld
// @ 0x00686A70 (manifest "CViewBase::GetDisplayEngine", propagated), CAnimateThing::UpdateFrameLogic
// @ 0x0070E8B0.
// Layout: Ego_r.pdb CTCClock (0x14): PDef +0x0c, LastTimeUpdated +0x10; the world's +0x1c record holds
// the normalised time of day at +8 (landed GetHandAnimationUpdateAmount 0x00732730).
// Shape: the out-parameter GetInterface with an uninitialised out pointer (a clear bit passes the
// stale frame word, `mov ecx,[esp+8]`); the animation is a named local (esi); the 12-byte update
// record {amount, 0, 0} is built from the float result (`fstp`) and the time of day is copied as a
// dword into LastTimeUpdated just before the update call.
struct CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};

struct CAnimFrameUpdate {                                   // 12-byte update record
    float Time;                                             // +0x00
    float Reserved0;                                        // +0x04
    float Reserved1;                                        // +0x08
    CAnimFrameUpdate(float time) : Time(time), Reserved0(0.0f), Reserved1(0.0f) {}
};

struct CAnimateThing {
    void UpdateFrameLogic(const CAnimFrameUpdate& update);  // 0x0070E8B0
};

struct CTCAnimationComplex {
    CAnimateThing* GetAnimation();                          // 0x0070B460
};

struct CTimeOfDay {
    unsigned char _pad[8];
    float         Time;                                     // +0x08 normalised time of day
};

struct CWorld {
    unsigned char _pad[0x1c];
    CTimeOfDay*   TimeOfDay;                                // +0x1c
};

struct CTCClock {
    virtual void v00();
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    unsigned char _pad08[4];
    void*         PDef;                                     // +0x0c CDefPointer<const CClockDef>
    float         LastTimeUpdated;                          // +0x10
    CWorld* GetWorld();                                     // 0x00686A70 (CTCBase)
    float GetHandAnimationUpdateAmount();                   // 0x00732730
    void FrameUpdate();
    CTCAnimationComplex* GetAnimationComplex()
    {
        CTCAnimationComplex* complex;
        if (ParentThing->HasInterface(0x5a))
            complex = (CTCAnimationComplex*)ParentThing->GetInterface(0x5a);
        return complex;
    }
};

void CTCClock::FrameUpdate()
{
    CAnimateThing* animation = GetAnimationComplex()->GetAnimation();
    CAnimFrameUpdate update(GetHandAnimationUpdateAmount());
    LastTimeUpdated = GetWorld()->TimeOfDay->Time;
    animation->UpdateFrameLogic(update);
}
