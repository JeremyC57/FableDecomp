// CVillageTaskPatrol::IsCreatureMoreAppropriateForTask(const CThing& creature) const -- unless the task is locked, true when the candidate is more than 1.25x nearer to its nearest patrol node than the assigned creature is to its own (squared distances: candidate * 1.5625 < assigned)
// Grade: DIFFER(302v302) -- scheduling only: retail loads all three dwords of the first two position copies before storing them, and puts fnstsw after the two dead fstp st(0) pops. Agent611: names re-proven (CThing::GetPos 0x004C73D0; GetNearestNode is const QBE -- the landed 0x00860A50 is QAE; engine/CVillageTaskPatrol.h layout through a derived method class). Tried: trivial copy ctor (376v302), assignment-body copy ctor, named float distances, named bool, if/return-true (tail order right but `mov al,1` and merged early exit, 294v302), ternary, int returns, swapped comparison.
// Leaf: ?IsCreatureMoreAppropriateForTask@CVillageTaskPatrol@@QBE_NABVCThing@@@Z
#define CVillageTaskPatrol CVillageTaskPatrolLayout
#include "engine/CVillageTaskPatrol.h"
#undef CVillageTaskPatrol
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const
    {
        C3DVector r;
        r.X = X - o.X;
        r.Y = Y - o.Y;
        r.Z = Z - o.Z;
        return r;
    }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
class CBaseObject;
class CThing { public:
    const C3DVector& GetPos() const;                     // 0x004C73D0
};
class CThingTrackNode : public CThing { };
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                       // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CVillageTaskPatrol : public CVillageTaskPatrolLayout { public:
    const CIntelligentPointer<const CThing>& GetCreatureAssignedToTask() const
    { return *reinterpret_cast<const CIntelligentPointer<const CThing>*>(CreatureAssignedToTask); }
    const CThingTrackNode* GetNearestNode(const C3DVector& position) const;   // 0x00860A50
    bool IsCreatureMoreAppropriateForTask(const CThing& creature) const;
};
bool CVillageTaskPatrol::IsCreatureMoreAppropriateForTask(const CThing& creature) const
{
    if (TaskLocked)
        return false;
    C3DVector assigned = GetCreatureAssignedToTask().GetPItem()->GetPos();
    C3DVector position = creature.GetPos();
    C3DVector assignedNode = GetNearestNode(assigned)->GetPos();
    C3DVector creatureNode = GetNearestNode(position)->GetPos();
    C3DVector assignedDelta = assignedNode - assigned;
    C3DVector creatureDelta = creatureNode - position;
    return creatureDelta.GetLengthSquared() * 1.5625f < assignedDelta.GetLengthSquared();
}
