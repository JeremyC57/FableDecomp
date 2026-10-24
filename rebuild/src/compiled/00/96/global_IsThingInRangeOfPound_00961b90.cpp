// IsThingInRangeOfPound(const CThing& creature, const CThing& target) -- the snow troll's ground-pound range test: true when the target's predicted position is within DistancePoundGroundRange (6.0 without a snow-troll def) of the creature in XY
// Leaf: ?IsThingInRangeOfPound@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00961B90, 177 bytes (+ int3 padding to 192), plain `ret`. Anonymous-namespace
//   __fastcall (Ego_r.pdb ?IsThingInRangeOfPound@?A0x9e6dcc01@@YI_NABVCThing@@0@Z): creature in ecx, target in edx.
// Callees: CDefClassBase::GetSubDef<CSnowTrollDef> @ 0x008C7A70, the def class's and the def's virtual slot 1 (count
//   release), GetPredictedTargetPosForThing @ 0x00961AB0 (same anonymous namespace; not inlined), CThing::GetPosition
//   @ 0x004C73D0.
// Shape: the range is a float local preset to 6.0f (`mov [esp+10h],40C00000h`) and overwritten from the def with an
//   integer move; the predicted position is a NAMED C3DVector local passed by hidden pointer (`lea ecx,[esp+1Ch]`);
//   `dx*dx + dy*dy < range*range` is `fcompp ; test ah,41h`, the bool kept in bl across the def release.
//   GetPredictedTargetPosForThing is only declared here so the call stays out of line, and a `_Use` caller keeps this
//   internal function emitted.
// Layout: Ego_r.pdb CSnowTrollDef::DistancePoundGroundRange +0x30; CThing counted def class +0x70.
// Label: catalog name confirmed (its callers are CAIStateGroup_IceTrollPoundGroundAttack::IsStillRunnable 0x00961C50
//   and ::IsRunnable 0x00961E80).
// Build: the lane defaults (/O2 /Oy /W3), no pragma.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
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
struct CTCPhysicsBase;
struct CThing {
    unsigned char _pad0[0x60];
    CTCPhysicsBase* m_pPhysics;                 // +0x60
    unsigned char _pad64[0x0c];
    CDefClassPointer PDefClass;                 // +0x70
    CDefClassPointer GetPDefClass() const { return PDefClass; }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

#pragma auto_inline(off)
long TmplSink(const void* self, long key);      // stand-in: the callee's own work (outside the graded bytes)
// Same-TU stand-in for GetSubDef<CSnowTrollDef> (retail 0x008C7A70), as in 0x00961AB0.
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointerSnowTroll* out) const
{
    CSnowTrollDef* old = out->m_p;
    out->m_p = (CSnowTrollDef*)TmplSink(this, (long)old);
    return out->m_p != 0;
}
#pragma auto_inline(on)

namespace {
C3DVector __fastcall GetPredictedTargetPosForThing(const CThing& creature, const CThing& target);   // 0x00961AB0
bool __fastcall IsThingInRangeOfPound(const CThing& creature, const CThing& target);
}
// Keeps the internal function emitted (VC7.1 discards an unreferenced one).
bool __fastcall IsThingInRangeOfPound_Use(const CThing& creature, const CThing& target)
{
    return IsThingInRangeOfPound(creature, target);
}
namespace {
bool __fastcall IsThingInRangeOfPound(const CThing& creature, const CThing& target)
{
    float range = 6.0f;
    CDefPointerSnowTroll def;
    if (creature.GetPDefClass()->GetSubDef(&def))
        range = def->DistancePoundGroundRange;
    C3DVector predicted = GetPredictedTargetPosForThing(creature, target);
    const C3DVector& me = creature.GetPosition();
    float dx = me.X - predicted.X;
    float dy = me.Y - predicted.Y;
    return dx * dx + dy * dy < range * range;
}
}