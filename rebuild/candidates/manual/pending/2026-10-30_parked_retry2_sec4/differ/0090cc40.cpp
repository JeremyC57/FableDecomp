// CAIStateGroup_MinionGossip::IsRunnable() -- not in a busy mode and gossip-capable: runnable when a nearby valid gossipee's gossip request (0x2c) has waited past the respond time, or when one of its gossip offers (0x2d) has waited past the initiate time (that message is removed); otherwise post the creature's own gossip offer unless one is already queued
// Grade: DIFFER(997v997) nd=10 (was 974v997 nd=305) -- own-message lookup in its own block (frame 0x48), named CMessageEventManager* before each FindMostRecentMessage, named fps/manager for AddMessage, IsOfType(0x2d) temporary in the binder; residue: HasTC(0xc1) creature in ecx v eax, RemoveMessage push edi before GetPWorld, &typeFilter store scheduled after the IsOriginatedByThing ctor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090CC40, length 1008 bytes (+ int3 padding), plain `ret`, frame 0x48.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (mode manager 0x31), mode manager GetCurrentMode @ 0x0073A6E0, GFGetCurrentFrame
// @ 0x0049D870, CThing::GetPosition @ 0x004C73D0, CThingCreatureBase::GetPosition @ 0x006A4D00, CThingFilter_IsNavLineOfSightToThing
// ctor @ 0x006A8710, CThingFilter_IsValidGossipee ctor @ 0x0090C870, CMessageEventFilter_IsOriginatedByThing ctor @ 0x006AF2B0,
// CMessageEventManager::FindMostRecentMessage<Bind4> @ 0x0090E2E0 / <Bind3 gossipee> @ 0x0090E3D0 / <Bind3 originated> @ 0x0090E090,
// RemoveMessage @ 0x00493B5B, AddMessage @ 0x00687540, CAIStateGroupBase::GetPWorld @ 0x008FCFC0.
// Layout: local model (PBrain +4; FrameOfLastGossip +0x14 from the engine header; world message manager +0x60; virtual
// GetTimeToInitiate slot 14, GetTimeToRespond slot 15, UseLineOfSight slot 16 from the rtti vtable 0x01288E34).
// Shape: the gossipee / originated filters are temporaries bound by const reference (their ctor's eax is stored), the line of
// sight filter is a named local.
extern long g_FramesPerSecond;                  // 0x01375550
long __fastcall GFGetCurrentFrame();             // 0x0049D870
struct C3DVector { float X, Y, Z; };
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition() const;       // 0x004C73D0
    bool HasTC(long t) const
    {
        if (m_presentBits[t >> 5] & (1u << (t & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1u << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
struct CThingAICreature : CThingCreatureBase { };
struct CCreatureMode {
    virtual void v00();
    virtual long GetType() const;               // slot 1
};
struct CTCCreatureModeManager {                 // interface 0x31
    CCreatureMode* GetCurrentMode() const;      // 0x0073A6E0
};
struct CMessageEvent;
struct CMessageEventFilter_IsOfType {
    long Type;
    CMessageEventFilter_IsOfType(long type) : Type(type) { }
};
struct CMessageEventFilter_IsOriginatedByThing {
    CThing* Thing;
    long ID;
    CMessageEventFilter_IsOriginatedByThing(CThing* thing);    // 0x006AF2B0
};
struct CThingFilter_IsNavLineOfSightToThing {
    CThing* Thing;
    long Extra;
    CThingFilter_IsNavLineOfSightToThing(CThing* thing);       // 0x006A8710
};
struct CThingFilter_IsValidGossipee {
    CThingAICreature* Thing;
    void* PFaction;
    void* PTCVillage;
    CThingFilter_IsValidGossipee(CThingAICreature* thing);     // 0x0090C870
};
struct CThingFilter_In2DRange {
    C3DVector Pos;
    float Range;
    __forceinline CThingFilter_In2DRange(const CThing* thing, float range) : Pos(thing->GetPosition()), Range(range) { }
};
template<class F> struct CMessageEventFilter_ThingSearchFilter {
    const F* Filter;
    CMessageEventFilter_ThingSearchFilter(const F& filter) : Filter(&filter) { }
};
template<class F1, class F2, class F3> struct CFilter_Bind3 {
    const F1* PF1; const F2* PF2; const F3* PF3;
    CFilter_Bind3(const F1& f1, const F2& f2, const F3& f3) : PF1(&f1), PF2(&f2), PF3(&f3) { }
};
template<class F1, class F2, class F3, class F4> struct CFilter_Bind4 {
    const F1* PF1; const F2* PF2; const F3* PF3; const F4* PF4;
    CFilter_Bind4(const F1& f1, const F2& f2, const F3& f3, const F4& f4) : PF1(&f1), PF2(&f2), PF3(&f3), PF4(&f4) { }
};
typedef CMessageEventFilter_ThingSearchFilter<CThingFilter_IsValidGossipee> TSFGossipee;
typedef CMessageEventFilter_ThingSearchFilter<CThingFilter_In2DRange> TSFRange;
typedef CMessageEventFilter_ThingSearchFilter<CThingFilter_IsNavLineOfSightToThing> TSFSight;
typedef CFilter_Bind4<CMessageEventFilter_IsOfType, TSFGossipee, TSFRange, TSFSight> CGossipSightFilter;
typedef CFilter_Bind3<CMessageEventFilter_IsOfType, TSFGossipee, TSFRange> CGossipFilter;
typedef CFilter_Bind3<CMessageEventFilter_IsOfType, CMessageEventFilter_IsOriginatedByThing, TSFRange> COwnMessageFilter;
struct CMessageEventManager {
    const CMessageEvent* FindMostRecentMessage(const CGossipSightFilter& filter) const;   // 0x0090E2E0
    const CMessageEvent* FindMostRecentMessage(const CGossipFilter& filter) const;        // 0x0090E3D0
    const CMessageEvent* FindMostRecentMessage(const COwnMessageFilter& filter) const;    // 0x0090E090
    void RemoveMessage(const CMessageEvent* message);                                     // 0x00493B5B
    void AddMessage(long type, unsigned long lifetime, const C3DVector* pos, CThing* from, CThing* to,
                    float range, float radius, long data);                                // 0x00687540
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageEventManager; // +0x60
    static long GetWorldFrame() { return GFGetCurrentFrame(); }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingAICreature* Creature;                 // +0x18
};
struct CAIStateGroupBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28(); virtual bool IsRunnable();
    virtual void v30(); virtual void v34();
    virtual long GetTimeToInitiate();           // slot 14
    virtual long GetTimeToRespond();            // slot 15
    virtual bool UseLineOfSight();              // slot 16
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingAICreature* GetCreature() const { return PBrain->Creature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
};
struct CAIStateGroup_MinionGossip : CAIStateGroupBase {
    long FrameOfLastGossip;                     // +0x14
    bool IsRunnable();
};

bool CAIStateGroup_MinionGossip::IsRunnable()
{
    CTCCreatureModeManager* modes;
    GetCreature()->GetInterface(&modes, 0x31);
    long mode = modes->GetCurrentMode()->GetType();
    if (mode == 0x22 || mode == 6 || mode == 5 || mode == 0x29)
        return false;
    if (!GetCreature()->HasTC(0xc1))
        return false;
    long framesPerSecond = g_FramesPerSecond;
    long seconds = (CWorld::GetWorldFrame() - FrameOfLastGossip) / framesPerSecond;
    if (seconds > GetTimeToRespond()) {
        const CMessageEvent* message;
        if (UseLineOfSight()) {
            CThingFilter_IsNavLineOfSightToThing sight(GetCreature());
            TSFSight sightFilter(sight);
            CThingFilter_In2DRange range(GetCreature(), 16.0f);
            TSFRange rangeFilter(range);
            TSFGossipee gossipeeFilter((CThingFilter_IsValidGossipee(GetCreature())));
            CMessageEventFilter_IsOfType typeFilter(0x2c);
            CGossipSightFilter filter(typeFilter, gossipeeFilter, rangeFilter, sightFilter);
            CMessageEventManager* manager = GetPWorld()->PMessageEventManager;
            message = manager->FindMostRecentMessage(filter);
        } else {
            CThingFilter_In2DRange range(GetCreature(), 16.0f);
            TSFRange rangeFilter(range);
            TSFGossipee gossipeeFilter((CThingFilter_IsValidGossipee(GetCreature())));
            CMessageEventFilter_IsOfType typeFilter(0x2c);
            CGossipFilter filter(typeFilter, gossipeeFilter, rangeFilter);
            CMessageEventManager* manager = GetPWorld()->PMessageEventManager;
            message = manager->FindMostRecentMessage(filter);
        }
        if (message)
            return true;
    }
    {
        const CMessageEvent* message;
            if (UseLineOfSight()) {
                CThingFilter_IsNavLineOfSightToThing sight(GetCreature());
                TSFSight sightFilter(sight);
                CThingFilter_In2DRange range(GetCreature(), 16.0f);
                TSFRange rangeFilter(range);
                TSFGossipee gossipeeFilter((CThingFilter_IsValidGossipee(GetCreature())));
                CMessageEventFilter_IsOfType typeFilter(0x2d);
                CGossipSightFilter filter(typeFilter, gossipeeFilter, rangeFilter, sightFilter);
                CMessageEventManager* manager = GetPWorld()->PMessageEventManager;
            message = manager->FindMostRecentMessage(filter);
            } else {
                CThingFilter_In2DRange range(GetCreature(), 16.0f);
                TSFRange rangeFilter(range);
                TSFGossipee gossipeeFilter((CThingFilter_IsValidGossipee(GetCreature())));
                CMessageEventFilter_IsOfType typeFilter(0x2d);
                CGossipFilter filter(typeFilter, gossipeeFilter, rangeFilter);
                CMessageEventManager* manager = GetPWorld()->PMessageEventManager;
            message = manager->FindMostRecentMessage(filter);
            }
        if (seconds > GetTimeToInitiate() && message) {
            GetPWorld()->PMessageEventManager->RemoveMessage(message);
            return true;
        }
    }
    {
        CThingFilter_In2DRange range(GetCreature(), 16.0f);
        TSFRange rangeFilter(range);
        COwnMessageFilter filter(CMessageEventFilter_IsOfType(0x2d), CMessageEventFilter_IsOriginatedByThing(GetCreature()), rangeFilter);
        CMessageEventManager* manager = GetPWorld()->PMessageEventManager;
        if (!manager->FindMostRecentMessage(filter)) {
            CThingAICreature* creature = GetCreature();
            long framesPerSecond = g_FramesPerSecond;
            CMessageEventManager* events = GetPWorld()->PMessageEventManager;
            events->AddMessage(0x2d, framesPerSecond * 4, &creature->GetPosition(), creature, 0,
                                                          16.0f, 16.0f, 0);
        }
    }
    return false;
}
