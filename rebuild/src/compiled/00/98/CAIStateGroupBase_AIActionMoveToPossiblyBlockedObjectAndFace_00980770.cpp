// CAIStateGroupBase::AIAction_MoveToPossiblyBlockedObjectAndFace(thing, index, proximity, seconds, filter, route, avoid, speed)
// Grade: RELOCATION_MATCH (1117v1117)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00980770, 1120 bytes, ret 0x24.
// Shape: alive + filter guard, user-action-point interface 0x2d, optional pre-calculated route, approach the thing,
//   then poll GetFreeActionPoint (minion wander while waiting, timeout = fps * seconds frames), require an unclaimed
//   interface 0x47, walk to the point and turn to face along its orientation; the point is freed on every failure.
// Notes: each `bool reached` lives in its own scope (retail packs it into the dead `route` argument slot); the
//   final branch is written inverted (`result != 2 || !filter()` first) so the route-failure return cross-jumps to
//   the same tail retail picks; retry loops use `if (r == 4) continue; break;`.
struct CTCBase;
struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(x + o.x, y + o.y, z + o.z); }
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
};
struct CRightHandedSet {
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
};
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
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline bool TestTC(long type) const { return ((m_presentBits[type >> 5] >> (type & 31)) & 1) != 0; }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
};
struct CThingBuilding;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointerThing& p);   // 0x004CC340
struct CTCUserActionPointBase {                 // interface 0x2d
    bool GetFreeActionPoint(CThing* user, C3DVector* pos, CRightHandedSet* orientation, long* index);   // 0x007FEDE0
    void FreeActionPoint(long index);           // 0x007FEF40
};
struct CTCClaim {                               // interface 0x47
    unsigned char _pad[0x10];
    void* Owner;                                // +0x10
};
struct CAIFilterBase {
    virtual bool operator()() const;            // slot 0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
extern long g_framesPerSecond;                  // 0x01375550
unsigned long GFGetCurrentFrame();              // 0x0049D870
struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool& reached, long movement, bool a, bool b,
        const CThing* thing, bool c, float speed);    // 0x0097EC00
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float proximity, bool& reached, long movement,
        bool a, bool b, bool c, const CThing* thing, bool d, float speed);         // 0x0097E890
    long AIAction_MoveToPreCalcRoute(const CThing& route, bool& reached, long movement, const CAIFilterBase& filter, bool a);   // 0x0097F690
    long AIAction_MinionWander(long movement, CThingBuilding* building);           // 0x0097F870
    long AIAction_MoveToPossiblyBlockedObjectAndFace(CIntelligentPointerThing thing, long& index, float proximity,
        float seconds, const CAIFilterBase& filter, const CThing* route, const CThing* avoid, float speed);
};

long CAIStateGroupBase::AIAction_MoveToPossiblyBlockedObjectAndFace(CIntelligentPointerThing thing, long& index, float proximity,
    float seconds, const CAIFilterBase& filter, const CThing* route, const CThing* avoid, float speed)
{
    if (thing.GetPItem() == 0 || (thing.GetPItem()->m_flags91 & 1) || !filter())
        return 3;
    CThing* target = thing.GetPItem();
    bool hasPoint = target->TestTC(0x2d);
    if (!hasPoint)
        return 3;
    CTCUserActionPointBase* point = (CTCUserActionPointBase*)target->GetTC(0x2d);
    long result;
    if (route) {
        bool reached;
        result = AIAction_MoveToPreCalcRoute(*route, reached, 3, filter, true);
        if (result != 2)
            return result;
    }
    for (;;) {
        if (thing.GetPItem() == 0 || (thing.GetPItem()->m_flags91 & 1) || !filter())
            return 3;
        CThing* item = thing.GetPItem();
        bool reached;
        result = AIAction_MoveToPosProximity(2, item->GetPosition(), proximity, reached, 3, true, false, true, 0, true, 0.0f);
        if (result == 2)
            break;
        if (result == 4)
            continue;
        return result;
    }
    if (!IS_THING_ALIVE(thing))
        return 3;
    long fps = g_framesPerSecond;
    unsigned long endFrame = GFGetCurrentFrame() + (long)(fps * seconds);
    C3DVector pos;
    CRightHandedSet orientation;
    while (!point->GetFreeActionPoint(GetCreature(), &pos, &orientation, &index)) {
        if (AIAction_MinionWander(3, 0) == 1)
            return 1;
        if (GFGetCurrentFrame() > endFrame)
            return 3;
        if (!IS_THING_ALIVE(thing) || !filter())
            return 3;
    }
    CThing* claimed = thing.GetPItem();
    bool hasClaim = claimed->TestTC(0x47);
    if (!hasClaim || ((CTCClaim*)claimed->GetTC(0x47))->Owner != 0) {
        point->FreeActionPoint(index);
        return 3;
    }
    for (;;) {
        if (!IS_THING_ALIVE(thing))
            return 3;
        if (!filter()) {
            point->FreeActionPoint(index);
            return 3;
        }
        bool reached;
        result = AIAction_MoveToPos(2, pos, reached, 3, true, true, avoid, true, speed);
        if (result == 4)
            continue;
        break;
    }
    if (!IS_THING_ALIVE(thing))
        return 3;
    if (result != 2 || !filter()) {
        point->FreeActionPoint(index);
        if (result == 2)
            result = 3;
        return result;
    }
    C3DVector forward = orientation.Forward;
    C3DVector lookPos = forward + PBrain->PCreature->GetPosition();
    for (;;) {
        if (!IS_THING_ALIVE(thing))
            return 3;
        if (!filter()) {
            point->FreeActionPoint(index);
            return 3;
        }
        result = AIAction_LookAtPos(2, lookPos, 0x11, 0.01f);
        if (result == 4)
            continue;
        break;
    }
    if (!IS_THING_ALIVE(thing))
        return 3;
    if (result == 2 && filter())
        return result;
    point->FreeActionPoint(index);
    if (result == 2)
        result = 3;
    return result;
}