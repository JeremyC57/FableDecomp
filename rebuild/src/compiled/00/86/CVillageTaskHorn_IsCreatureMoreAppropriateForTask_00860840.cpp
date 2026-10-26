// CVillageTaskHorn::IsCreatureMoreAppropriateForTask(const CThing&) const -- true when the candidate creature is more than 1.25x nearer to the horn than the creature currently assigned (squared distances: candidate * 1.5625 < assigned)
// Leaf: IsCreatureMoreAppropriateForTask
// Retail: Fable.exe (TLC, VC7.1) @ 0x00860840, 247 bytes (256 with padding), `ret 4`.
//   PDB ?IsCreatureMoreAppropriateForTask@CVillageTaskHorn@@QBE_NABVCThing@@@Z (non-virtual).
// Identity: the manifest labels this row "CVillageTaskHorn::IsCreatureMoreAppropriateForTask" (propagated across the
//   byte twins 0x00860650 / 0x00860840); by address it sits in the CVillageTaskHorn block (after Horn AssignTarget 0x00860750 / RemoveThingFromTask 0x008607A0), and the PDB lists the
//   method for Torch, Horn, Patrol, SecuritySweep and CarryCrate.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x2), CThing::GetPosition 0x004C73D0 (x3). Constant 1.5625f
//   0x01276D70 (a literal).
// Layout (Ego_r.pdb = retail): PHorn +0x08, CreatureAssignedToTask +0x10.
// Shape: the landed CVillageTaskSecuritySweep::IsCreatureMoreAppropriateForTask (0x00860ED0) with the goal point taken
//   from the PHorn thing: all three positions are by-value copies (the candidate's X/Y kept on the x87 stack), the
//   two deltas named locals, the candidate side of `<` evaluated first.
class CBaseObject { public: };
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
class CThing : public CBaseObject { public:
    const C3DVector& GetPosition() const;          // 0x004C73D0
};
struct CBaseIntelligentPointer {                   // CIntelligentPointer<T>: vptr + item
    virtual void v00();
    CBaseObject* m_pItem;                          // +0x04
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};
class CVillageTaskHorn { public:                         // vtable 0x0125A8D8
    virtual ~CVillageTaskHorn();
    void* m_pVillage;                              // +0x04 (CVillageTaskBase)
    CBaseIntelligentPointer PHorn;                 // +0x08 CIntelligentPointer<CThing>
    CBaseIntelligentPointer CreatureAssignedToTask;   // +0x10 CIntelligentPointer<const CThing>
    bool IsCreatureMoreAppropriateForTask(const CThing& creature) const;
};
bool CVillageTaskHorn::IsCreatureMoreAppropriateForTask(const CThing& creature) const
{
    C3DVector horn = static_cast<const CThing*>(PHorn.GetPItem())->GetPosition();
    C3DVector assigned = static_cast<const CThing*>(CreatureAssignedToTask.GetPItem())->GetPosition();
    C3DVector position = creature.GetPosition();
    C3DVector assignedDelta = horn - assigned;
    C3DVector creatureDelta = horn - position;
    return creatureDelta.GetLengthSquared() * 1.5625f < assignedDelta.GetLengthSquared();
}