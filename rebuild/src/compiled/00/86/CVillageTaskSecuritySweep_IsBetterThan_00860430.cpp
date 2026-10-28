// CVillageTaskSecuritySweep::IsBetterThan(const CVillageTaskSecuritySweep* other, const CThing& thing) -- with another task to compare against (and this one available) this task wins when its sweep point is nearer to the thing; with no rival the answer is IsTaskAvailable()
// Leaf: IsBetterThan
// Retail: Fable.exe (TLC, VC7.1) @ 0x00860430, 204 bytes (padded to 208), `ret 8`.
//   PDB name ?IsBetterThan@CVillageTaskSecuritySweep@@QBE_NPBV1@ABVCThing@@@Z.
// Callees: virtual IsTaskAvailable (slot 4, +0x10 of vtable 0x0125A868), CThing::GetPos 0x004C73D0.
// Layout (engine/CVillageTaskSecuritySweep.h): CreatureAssignedToTask +0x08, PositionToGoTo +0x10.
// Shape: the thing's position is a by-value copy (X/Y kept on the x87 stack, Z copied through ecx); the two deltas
//   are named locals (this one spilled, the rival's mostly on the stack); `myDist < rivalDist` evaluates left first
//   (`fcompp ; test ah,41h ; jne`).
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
struct CBaseIntelligentPointer {                   // CIntelligentPointer<const CThing>: vptr + item
    virtual void v00();
    CBaseObject* m_pItem;                          // +0x04
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};
class CVillageTaskSecuritySweep { public:                 // vtable 0x0125A868
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
bool CVillageTaskSecuritySweep::IsBetterThan(const CVillageTaskSecuritySweep* other, const CThing& thing) const
{
    if (other && IsTaskAvailable()) {
        C3DVector position = thing.GetPos();
        C3DVector myDelta = PositionToGoTo - position;
        C3DVector rivalDelta = other->PositionToGoTo - position;
        return myDelta.GetLengthSquared() < rivalDelta.GetLengthSquared();
    }
    return IsTaskAvailable();
}