// CAIStateGroupBase::AIAction_MinionWander(EMovementType, CThingBuilding*) -- look around, pick a wander position (inside the building's bounds, up to ten tries, or within 3 of the creature) and walk there; otherwise yield the brain
// Grade: DIFFER(395v397) IMPROVED (14 -> 4 difflines): 'navigation = 0' gives retail's colouring (navigation ebx, building ebp) but replaces the miss path's read of the never-written home slot (retail: 'jmp; mov ebx,[esp+40h]', navigation homed in the dead building argument slot that radius reuses) with 'xor ebx,ebx'. Tried: building copied to a local (=), navigation declared at the top (=). Next: keep navigation uninitialised but give it an earlier use so it is allocated before building (e.g. a GetInterfaceT form returning through the slot).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097F870, length 400 bytes (`ret 8`).
// Callees: CAIStateGroupBase::IsStopping @ 0x008FD010 (x2), CVectorMap::LowerBound @ 0x0040F020 (interfaces 0x43 CTCLook,
//          7 CTCCreatureNavigation), CTCLook::TrackNearestInterestingThing(bool) @ 0x00753840, CTCPhysicsBase::
//          GetBoundingInfo(C3DVector*, float*) @ 0x006AF640, CTCCreatureNavigation::GetNearbyWanderPosition @ 0x006CB6E0,
//          CThingBuilding::IsPointInside @ 0x0082D000, CThing::GetPosition const @ 0x006A4D00, AIAction_MoveToPos @
//          0x0097EC00, the brain's virtual slot 2 (+8, yield).
// Shape: the creature is a named local (edi, reused for the else-branch position); the physics TC is a named local; both interfaces use the bitset::test GetInterfaceT form (`shr ; and cl,1` kept in a frame byte); the result starts
//          at 3 and is the move's return; a stopping group always reports 1.
// Layout: CAIStateGroupBase modelled locally as engine/CAIStateGroupBase.h (PBrain +4); the building's physics TC +0x60.
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
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
    const C3DVector& GetPosition() const;       // 0x006A4D00
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return has;
    }
};
struct CTCPhysicsBase {
    void GetBoundingInfo(C3DVector* centre, float* radius);   // 0x006AF640
};
struct CThingBuilding : CThing {
    unsigned char _pad1[0x60 - sizeof(CThing)];
    CTCPhysicsBase* PPhysics;                   // +0x60
    bool IsPointInside(const C3DVector* point); // 0x0082D000
};
struct CTCLook {                                // interface 0x43
    bool TrackNearestInterestingThing(bool track);   // 0x00753840
};
struct CTCCreatureNavigation {                  // interface 7
    bool GetNearbyWanderPosition(const C3DVector* centre, float radius, C3DVector* result, bool flag);   // 0x006CB6E0
};
struct CAIBrain {
    virtual void v00();
    virtual void v01();
    virtual void Yield();                       // slot 2 (+8)
    unsigned char _pad04[0x18 - 0x04];
    CThing* PCreature;                          // +0x18
};
enum EAIActionRepeat { AI_ACTION_ONCE = 0, AI_ACTION_REPEAT = 1 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_FAILED = 3, AI_ACTION_RUNNING = 4 };
enum EMovementType { };

struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThing* GetCreature() const { return PBrain->PCreature; }
    bool IsStopping();                          // 0x008FD010
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool* reached, EMovementType movement,
        bool a, bool b, CThing* avoid, bool c, float speed);   // 0x0097EC00
    EAIActionReturn AIAction_MinionWander(EMovementType movement, CThingBuilding* building);
};

EAIActionReturn CAIStateGroupBase::AIAction_MinionWander(EMovementType movement, CThingBuilding* building)
{
    if (IsStopping())
        return AI_ACTION_STOPPING;
    EAIActionReturn result = AI_ACTION_FAILED;
    CTCLook* look;
    if (GetCreature()->GetInterfaceT(&look, 0x43))
        look->TrackNearestInterestingThing(true);
    CThing* creature = GetCreature();
    CTCCreatureNavigation* navigation = 0;
    creature->GetInterfaceT(&navigation, 7);
    C3DVector wanderPos;
    if (building) {
        C3DVector centre;
        float radius;
        CTCPhysicsBase* physics = building->PPhysics;
        physics->GetBoundingInfo(&centre, &radius);
        for (long i = 0; i < 10; ++i) {
            if (navigation->GetNearbyWanderPosition(&centre, radius, &wanderPos, false)
                && building->IsPointInside(&wanderPos))
                goto move;
        }
    } else if (navigation->GetNearbyWanderPosition(&creature->GetPosition(), 3.0f, &wanderPos, true))
        goto move;
    PBrain->Yield();
    goto done;
move:
    {
        bool reached;
        result = AIAction_MoveToPos(AI_ACTION_REPEAT, wanderPos, &reached, movement, true, true, 0, true, 0.0f);
    }
done:
    if (IsStopping())
        return AI_ACTION_STOPPING;
    return result;
}
