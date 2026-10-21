// CAIStateGroup_MinionWatchFires::Update() -- walk to the burning building while it burns, then keep playing the alert-to-player action until it stops
// Leaf: Update
// Grade: DIFFER(442v438) -- close (10 diff lines): the inlined STLport lower_bound loads first into eax and copies it to esi (ours loads esi directly, which also shifts the loop alignment nop), and the this-reload / shared Terminate entry of the tail loop is scheduled differently
// Retail: Fable.exe (TLC, VC7.1) @ 0x009363D0; best of the probes graded under VC7.1 RTM (probe/009363d0e.cpp).
struct CTCBase;
struct C3DVector {
    float x, y, z;
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
inline CInterfaceMap::SEntry* LowerBoundInline(CInterfaceMap::SEntry* first, CInterfaceMap::SEntry* last, long key)
{
    long len = last - first;
    while (len > 0) {
        long half = len / 2;
        CInterfaceMap::SEntry* middle = first + half;
        if (middle->type < key) {
            first = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return first;
}
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterfaceInline(T** ppOut, long interfaceType)
    {
        bool has = (unsigned char)(m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1;
        if (has) {
            CInterfaceMap::SEntry* last = m_interfaces.last;
            CInterfaceMap::SEntry* it = LowerBoundInline(m_interfaces.first, last, interfaceType);
            *ppOut = (T*)((it == last || interfaceType < it->type) ? last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CTCFlammable {                           // interface 0x6d
    bool IsOnFire();                            // 0x00780400
};
struct CThingAICreature;
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_AlertToPlayer : CCreatureActionBase {
    CCreatureAction_AlertToPlayer(CThingAICreature* creature);    // 0x00842270
};
struct CThingCreature : CThing { };
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreature* PCreature;                  // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    void Terminate();                           // 0x008FCF40
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float proximity, bool* reached, long movement,
        bool a, bool b, bool c, CThing* thing, bool d, float speed);   // 0x0097E890
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool b);   // 0x0097DB70
    long AIAction_DoNothing();                  // 0x0097E200
};
struct CAIStateGroup_MinionWatchFires : CAIStateGroupBase {
    CBaseIntelligentPointer PBuildingOnFire;    // +0x14
    virtual void Update();
};

void CAIStateGroup_MinionWatchFires::Update()
{
    bool reached = false;
    long result;
    do {
        if (PBuildingOnFire.GetPItem() == 0 || (PBuildingOnFire.GetPItem()->m_flags91 & 1))
            goto stop;
        CTCFlammable* flammable;
        if (!PBuildingOnFire.GetPItem()->GetInterfaceInline(&flammable, 0x6d) || !flammable->IsOnFire())
            goto stop;
        CThing* building = PBuildingOnFire.GetPItem();
        result = AIAction_MoveToPosProximity(2, building->GetPosition(), 5.0f, &reached, 5,
            true, false, true, 0, true, 0.0f);
    } while (result == 4);
    if (result != 2)
        goto stop;
    do {
        if (PBuildingOnFire.GetPItem() == 0 || (PBuildingOnFire.GetPItem()->m_flags91 & 1))
            goto stop;
        CTCFlammable* flammable;
        if (!PBuildingOnFire.GetPItem()->GetInterface(&flammable, 0x6d) || !flammable->IsOnFire())
            goto stop;
        if (AIAction_PerformActionAndWait(&CCreatureAction_AlertToPlayer((CThingAICreature*)PBrain->PCreature), false) != 2)
            goto stop;
    } while (AIAction_DoNothing() == 4);
    return;
stop:
    Terminate();
}
