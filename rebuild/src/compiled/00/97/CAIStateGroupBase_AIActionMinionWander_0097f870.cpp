// CAIStateGroupBase::AIAction_MinionWander(EMovementType, CThingBuilding*) -- look around, pick a wander position (inside the building's bounds, up to ten tries, or within 3 of the creature) and walk there; otherwise yield the brain
// Leaf: ?AIAction_MinionWander@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EMovementType@@PAVCThingBuilding@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097F870, length 400 bytes (`ret 8`).
// Callees: CAIStateGroupBase::IsStopping @ 0x008FD010 (x2), CVectorMap::LowerBound @ 0x0040F020 (interfaces 0x43 CTCLook,
//          7 CTCCreatureNavigation), CTCLook::TrackNearestInterestingThing(bool) @ 0x00753840, CTCPhysicsBase::
//          GetBoundingInfo(C3DVector*, float*) @ 0x006AF640, CTCCreatureNavigation::GetNearbyWanderPosition @ 0x006CB6E0,
//          CThingBuilding::IsPointInside @ 0x0082D000, CThing::GetPosition const @ 0x006A4D00, AIAction_MoveToPos @
//          0x0097EC00, the brain's virtual slot 2 (+8, yield).
// Fix (chunk 762): the found flag (`bool found`, set in the building loop / the else-branch result, then `if (found) move;
//          else Yield`) instead of two gotos gives retail's navigation=ebx / building=ebp; PeekTC over the real engine/CVectorMap.h.
// Shape: the creature is a named local (edi, reused for the else-branch position); the physics TC is a named local; both interfaces use the bitset::test GetInterfaceT form (`shr ; and cl,1` kept in a frame byte); the result starts
//          at 3 and is the move's return; a stopping group always reports 1.
// Layout: CAIStateGroupBase modelled locally as engine/CAIStateGroupBase.h (PBrain +4); the building's physics TC +0x60.
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
class CTCBase { public: virtual ~CTCBase(); };
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CThing { public:
    unsigned char _pad0[0x20];
    std::bitset<274> UsingComponents;           // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template<class T> __forceinline bool PeekTC(ETCInterfaceType id, T** ptc)
    {
        if (HasTC(id)) {
            ETCInterfaceType type = id;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(type);
            if (it == map->end() || id < it->first)
                it = map->end();
            *ptc = static_cast<T*>(it->second);
            return true;
        }
        return false;
    }
};
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;           // 0x006A4D00 ?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ
};
class CThingAICreature : public CThingPhysical { public: };
class CTCPhysicsBase { public:
    void GetBoundingInfo(C3DVector& centre, float& radius) const;   // 0x006AF640
};
class CThingBuilding : public CThingPhysical { public:
    unsigned char _pad1[0x60 - sizeof(CThing)];
    CTCPhysicsBase* PhysicsTC;                  // +0x60 CThing::PhysicsTC (retail offset)
    bool IsPointInside(const C3DVector& point) const; // 0x0082D000
};
class CTCLook : public CTCBase { public:                                // interface 0x43
    bool TrackNearestInterestingThing(bool track);   // 0x00753840
};
class CTCCreatureNavigation : public CTCBase { public:                  // interface 7
    bool GetNearbyWanderPosition(const C3DVector& centre, float radius, C3DVector& result, bool flag) const;   // 0x006CB6E0
};
class CAIBrain { public:
    virtual void v00();
    virtual void v01();
    virtual void Yield();                       // slot 2 (+8)
    unsigned char _pad04[0x18 - 0x04];
    CThingAICreature* Creature;                 // +0x18
};
enum EAIActionRepeat { AI_ACTION_ONCE = 0, AI_ACTION_REPEAT = 1 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_FAILED = 3, AI_ACTION_RUNNING = 4 };
enum EMovementType { };

class CAIStateGroupBase { public:
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
protected:
    CThingAICreature* GetCreature() const { return PBrain->Creature; }
    bool IsStopping() const;                    // 0x008FD010 ?IsStopping@CAIStateGroupBase@@IBE_NXZ
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool& reached, EMovementType movement,
        bool a, bool b, const CThing* avoid, bool c, float speed);   // 0x0097EC00
    EAIActionReturn AIAction_MinionWander(EMovementType movement, CThingBuilding* building);
};

EAIActionReturn CAIStateGroupBase::AIAction_MinionWander(EMovementType movement, CThingBuilding* building)
{
    if (IsStopping())
        return AI_ACTION_STOPPING;
    EAIActionReturn result = AI_ACTION_FAILED;
    CTCLook* look;
    if (GetCreature()->PeekTC(TCI_LOOK, &look))
        look->TrackNearestInterestingThing(true);
    CThingAICreature* creature = GetCreature();
    CTCCreatureNavigation* navigation;
    creature->PeekTC(TCI_NAVIGATION, &navigation);
    C3DVector wanderPos;
    bool found = false;
    if (building) {
        C3DVector centre;
        float radius;
        CTCPhysicsBase* physics = building->PhysicsTC;
        physics->GetBoundingInfo(centre, radius);
        for (long i = 0; i < 10; ++i) {
            if (navigation->GetNearbyWanderPosition(centre, radius, wanderPos, false)
                && building->IsPointInside(wanderPos)) {
                found = true;
                break;
            }
        }
    } else
        found = navigation->GetNearbyWanderPosition(creature->PeekPos(), 3.0f, wanderPos, true);
    if (found) {
        bool reached;
        result = AIAction_MoveToPos(AI_ACTION_REPEAT, wanderPos, reached, movement, true, true, 0, true, 0.0f);
    } else
        PBrain->Yield();
    if (IsStopping())
        return AI_ACTION_STOPPING;
    return result;
}