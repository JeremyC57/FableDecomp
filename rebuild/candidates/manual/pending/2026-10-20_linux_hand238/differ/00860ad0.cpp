// Grade: DIFFER(302v302) -- scheduling only: retail loads all three dwords of the first two position copies before storing, and places fnstsw after the two fstp st(0); ours interleaves
// CVillageTaskPatrol::IsCreatureMoreAppropriateForTask(const CThing&) const -- unless the task is locked, true when the candidate is more than 1.25x nearer to its nearest patrol node than the assigned creature is to its own (squared distances: candidate * 1.5625 < assigned)
// Leaf: IsCreatureMoreAppropriateForTask
struct CBaseObject { };
struct C3DVector {
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
struct CThing : CBaseObject {
    const C3DVector& GetPosition() const;          // 0x004C73D0
};
struct CBaseIntelligentPointer {                   // CIntelligentPointer<T>: vptr + item
    virtual void v00();
    CBaseObject* m_pItem;                          // +0x04
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};
struct CThingTrackNode : CThing { };
struct CVillageTaskPatrol {                        // vtable 0x0125A900
    virtual ~CVillageTaskPatrol();
    void* m_pVillage;                              // +0x04 (CVillageTaskBase)
    CBaseIntelligentPointer PatrolStart;           // +0x08 CIntelligentPointer<const CThingTrackNode>
    CBaseIntelligentPointer CreatureAssignedToTask;   // +0x10 CIntelligentPointer<const CThing>
    bool TaskLocked;                               // +0x18
    const CThingTrackNode* GetNearestNode(const C3DVector& position) const;   // 0x00860A50 (landed)
    bool IsCreatureMoreAppropriateForTask(const CThing& creature) const;
};
bool CVillageTaskPatrol::IsCreatureMoreAppropriateForTask(const CThing& creature) const
{
    if (TaskLocked)
        return false;
    C3DVector assigned = static_cast<const CThing*>(CreatureAssignedToTask.GetPItem())->GetPosition();
    C3DVector position = creature.GetPosition();
    C3DVector assignedNode = GetNearestNode(assigned)->GetPosition();
    C3DVector creatureNode = GetNearestNode(position)->GetPosition();
    C3DVector assignedDelta = assignedNode - assigned;
    C3DVector creatureDelta = creatureNode - position;
    return creatureDelta.GetLengthSquared() * 1.5625f < assignedDelta.GetLengthSquared();
}
