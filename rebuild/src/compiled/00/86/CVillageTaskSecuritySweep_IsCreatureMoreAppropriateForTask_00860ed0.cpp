// CVillageTaskSecuritySweep::IsCreatureMoreAppropriateForTask(const CThing& creature) -- true when the candidate creature is more than 1.25x nearer to the sweep point than the creature currently assigned (squared distances: candidate * 1.5625 < assigned)
// Leaf: IsCreatureMoreAppropriateForTask
// Retail: Fable.exe (TLC, VC7.1) @ 0x00860ED0, 205 bytes (padded to 208), `ret 4`.
//   PDB name ?IsCreatureMoreAppropriateForTask@CVillageTaskSecuritySweep@@QBE_NABVCThing@@@Z.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (CreatureAssignedToTask +0x08), CThing::GetPosition 0x004C73D0
//   (x2). Constant 1.5625f 0x01276D70 (a literal: `x * k` loads x first, then `fmul [pool]`).
// Layout (engine/CVillageTaskSecuritySweep.h): CreatureAssignedToTask +0x08, PositionToGoTo +0x10.
// Shape: both positions are by-value copies of the const references (the assigned one fully copied through integer
//   registers, the candidate's X/Y kept on the x87 stack); the deltas are named locals; the candidate side of the
//   `<` is evaluated first (left operand), as in the IsBetterThan twin 0x00860430.
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
struct CBaseIntelligentPointer {                   // CIntelligentPointer<const CThing>: vptr + item
    virtual void v00();
    CBaseObject* m_pItem;                          // +0x04
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};
struct CVillageTaskSecuritySweep {                 // vtable 0x0125A868
    virtual ~CVillageTaskSecuritySweep();          // slot 0
    virtual void v01();
    virtual long GetTaskType() const;              // slot 2 0x008602F0
    virtual bool IsValid() const;                  // slot 3 0x00860300
    virtual bool IsTaskAvailable() const;          // slot 4 0x00860FC0 (+0x10)
    void* m_pVillage;                              // +0x04 (CVillageTaskBase)
    CBaseIntelligentPointer CreatureAssignedToTask;   // +0x08
    C3DVector PositionToGoTo;                      // +0x10
    float RadiusAroundThePositionToStay;           // +0x1c
    C3DVector PositionToLookTowards;               // +0x20
    float AngleToLookTowards;                      // +0x2c
    bool HasBecomeInvalid;                         // +0x30
    bool IsBetterThan(const CVillageTaskSecuritySweep* other, const CThing& thing) const;
    bool IsCreatureMoreAppropriateForTask(const CThing& creature) const;
};
bool CVillageTaskSecuritySweep::IsCreatureMoreAppropriateForTask(const CThing& creature) const
{
    C3DVector assigned = static_cast<const CThing*>(CreatureAssignedToTask.GetPItem())->GetPosition();
    C3DVector position = creature.GetPosition();
    C3DVector assignedDelta = PositionToGoTo - assigned;
    C3DVector creatureDelta = PositionToGoTo - position;
    return creatureDelta.GetLengthSquared() * 1.5625f < assignedDelta.GetLengthSquared();
}