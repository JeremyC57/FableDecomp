// CVillageTaskCarryCrate::IsCreatureMoreAppropriateForTask(const CThing&) const -- for a pickup task, true when the candidate is valid for the task and more than 1.5x nearer to the crate stack than the assigned creature (squared distances, factor 2.25)
// Leaf: IsCreatureMoreAppropriateForTask
// Retail: Fable.exe (TLC, VC7.1) @ 0x00861530, 313 bytes (320 with padding), `ret 4`.
//   PDB ?IsCreatureMoreAppropriateForTask@CVillageTaskCarryCrate@@QBE_NABVCThing@@@Z (non-virtual).
// Identity: labelled "CVillageTaskHorn::IsCreatureMoreAppropriateForTask" (propagated); it reads Pickup at +0x18 and
//   calls the CarryCrate validity test 0x008614B0, after CarryCrate IsValid 0x00861430 by address.
// Callees: CVillageTaskCarryCrate::IsCreatureValidForTask 0x008614B0, CBaseIntelligentPointer::GetPItem 0x00A01B50
//   (x2), CThing::GetPos 0x004C73D0 (x3). Constant 2.25f 0x0125B2D4 (a literal, both uses).
// Layout (Ego_r.pdb = retail): CreatureAssignedToTask +0x08, PCrateStack +0x10, Pickup +0x18, Priority +0x1C.
// Shape: the Torch/Horn twins' distance code with the assigned distance a named float (stored into the dead `creature`
//   slot) and the candidate distance kept on the x87 stack; Pickup is re-read after the validity call, so both arms of
//   the final comparison survive (`2.25 * c < a` via `test ah,5 ; jnp`, `c > a * 2.25` via `test ah,41h ; jne`).
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
    const C3DVector& GetPos() const;          // 0x004C73D0
};
struct CBaseIntelligentPointer {                   // CIntelligentPointer<T>: vptr + item
    virtual void v00();
    CBaseObject* m_pItem;                          // +0x04
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};
class CVillageTaskCarryCrate { public:                    // vtable 0x0125A950
    virtual ~CVillageTaskCarryCrate();
    void* m_pVillage;                              // +0x04 (CVillageTaskBase)
    CBaseIntelligentPointer CreatureAssignedToTask;   // +0x08 CIntelligentPointer<const CThing>
    CBaseIntelligentPointer PCrateStack;           // +0x10 CIntelligentPointer<CThing>
    bool Pickup;                                   // +0x18
    long Priority;                                 // +0x1C
    bool IsCreatureValidForTask(const CThing& creature) const;   // 0x008614B0
    bool IsCreatureMoreAppropriateForTask(const CThing& creature) const;
};
bool CVillageTaskCarryCrate::IsCreatureMoreAppropriateForTask(const CThing& creature) const
{
    bool result = false;
    if (!Pickup)
        return false;
    if (IsCreatureValidForTask(creature)) {
        C3DVector crate = static_cast<const CThing*>(PCrateStack.GetPItem())->GetPos();
        C3DVector assigned = static_cast<const CThing*>(CreatureAssignedToTask.GetPItem())->GetPos();
        C3DVector position = creature.GetPos();
        float assignedDistance = (crate - assigned).GetLengthSquared();
        float creatureDistance = (crate - position).GetLengthSquared();
        if (Pickup)
            result = creatureDistance * 2.25f < assignedDistance;
        else
            result = creatureDistance > assignedDistance * 2.25f;
    }
    return result;
}