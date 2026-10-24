// GetPredictedTargetPosForThing(const CThing& creature, const CThing& target) -- the snow troll's icicle-attack aim point: the target's position extrapolated along its last-frame movement by the creature's SecondsIcicleAttackTargetPositionPredictionTime (2.0 without a snow-troll def)
// Leaf: ?GetPredictedTargetPosForThing@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00961AB0, 221 bytes (+ int3 padding to 224), `ret 4`. Anonymous-namespace
//   __fastcall (Ego_r.pdb ?GetPredictedTargetPosForThing@?A0x9e6dcc01@@YI?AVC3DVector@@ABVCThing@@0@Z): the C3DVector
//   result by hidden pointer in ecx, creature in edx, target on the stack.
// Callees: CDefClassBase::GetSubDef<CSnowTrollDef> @ 0x008C7A70, the def class's and the def's virtual slot 1 (the
//   count release when a reference count reaches zero).
// Shape: the time is a float local preset to 2.0f and overwritten from the def when GetSubDef succeeds (`fld [ecx+2Ch]`
//   / `fld [esp+10h]`); the creature's counted def class (+0x70) is a by-value temporary released right after the call
//   and the def pointer is released at the end. The prediction is `position += velocity * time; return position;`
//   with velocity = position - OldPosition: the `+=` form reuses the position local for the result (its Z is stored
//   back to the position slot), where `return position + velocity * time` gives the sum its own slot. GetSubDef is
//   a same-TU auto_inline(off) stand-in (the accepted 0x00961FB0 idiom) so the def stays in a register. A `_Use`
//   caller sits before the definition so VC7.1 emits the internal function.
// Layout: Ego_r.pdb CSnowTrollDef::SecondsIcicleAttackTargetPositionPredictionTime +0x2c; CTCPhysicsBase Position +0x0c,
//   OldPosition +0x18; CThing physics component +0x60 and counted def class +0x70 (as the landed CThing models).
// Label: catalog name confirmed (Ego_r.pdb anonymous-namespace function of the ice-troll AI TU).
// Build: the lane defaults (/O2 /Oy /W3), no pragma.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    C3DVector operator-(const C3DVector& other) const { return C3DVector(X - other.X, Y - other.Y, Z - other.Z); }
    C3DVector operator+(const C3DVector& other) const { return C3DVector(X + other.X, Y + other.Y, Z + other.Z); }
    C3DVector operator*(float scale) const { return C3DVector(X * scale, Y * scale, Z * scale); }
    C3DVector& operator+=(const C3DVector& other) { X += other.X; Y += other.Y; Z += other.Z; return *this; }
};

struct CSnowTrollDef {
    virtual void v00();
    virtual void DeleteThis();                  // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0x24];
    float SecondsIcicleAttackTargetPositionPredictionTime;   // +0x2c
    float DistancePoundGroundRange;             // +0x30
    void Release() { if (--RefCount == 0) DeleteThis(); }
};
struct CDefPointerSnowTroll {                   // CDefPointer<const CSnowTrollDef>
    CSnowTrollDef* m_p;
    CDefPointerSnowTroll() : m_p(0) { }
    ~CDefPointerSnowTroll() { if (m_p) m_p->Release(); }
    CSnowTrollDef* operator->() const { return m_p; }
};
struct CDefClassBase {
    virtual void v00();
    virtual void DeleteThis();                  // slot 1
    long RefCount;                              // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) DeleteThis(); }
    bool GetSubDef(CDefPointerSnowTroll* out) const;   // 0x008C7A70 (GetSubDef<CSnowTrollDef>)
};
struct CDefClassPointer {                       // the counted def class pointer
    CDefClassBase* m_p;
    CDefClassPointer(const CDefClassPointer& other) : m_p(other.m_p) { if (m_p) m_p->AddRef(); }
    ~CDefClassPointer() { if (m_p) m_p->Release(); }
    CDefClassBase* operator->() const { return m_p; }
};
struct CTCPhysicsBase {
    unsigned char _pad00[0x0c];
    C3DVector Position;                         // +0x0c
    C3DVector OldPosition;                      // +0x18
};
struct CThing {
    unsigned char _pad0[0x60];
    CTCPhysicsBase* m_pPhysics;                 // +0x60
    unsigned char _pad64[0x0c];
    CDefClassPointer PDefClass;                 // +0x70
    CTCPhysicsBase* GetPhysics() const { return m_pPhysics; }
    CDefClassPointer GetPDefClass() const { return PDefClass; }
};

#pragma auto_inline(off)
long TmplSink(const void* self, long key);      // stand-in: the callee's own work (outside the graded bytes)
// Same-TU stand-in for GetSubDef<CSnowTrollDef> (retail 0x008C7A70): it reads the old out pointer and writes the new
// one, like CDefPointer::operator=, so the def's address does not escape.
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointerSnowTroll* out) const
{
    CSnowTrollDef* old = out->m_p;
    out->m_p = (CSnowTrollDef*)TmplSink(this, (long)old);
    return out->m_p != 0;
}
#pragma auto_inline(on)

namespace {
C3DVector __fastcall GetPredictedTargetPosForThing(const CThing& creature, const CThing& target);
}
// Keeps the internal function emitted (VC7.1 discards an unreferenced one); declared before the definition so the
// call is not inlined.
C3DVector __fastcall GetPredictedTargetPosForThing_Use(const CThing& creature, const CThing& target)
{
    return GetPredictedTargetPosForThing(creature, target);
}
namespace {
C3DVector __fastcall GetPredictedTargetPosForThing(const CThing& creature, const CThing& target)
{
    float predictionTime = 2.0f;
    CDefPointerSnowTroll def;
    if (creature.GetPDefClass()->GetSubDef(&def))
        predictionTime = def->SecondsIcicleAttackTargetPositionPredictionTime;
    C3DVector position = target.GetPhysics()->Position;
    C3DVector velocity = position - target.GetPhysics()->OldPosition;
    position += velocity * predictionTime;
    return position;
}
}