// GetRenderPositionForThing(const CThing& thing, float interpolation) -- the thing's render position: its physics position interpolated from last frame's position (exact when interpolation == 1), raised by the physics render Z offset for creatures
// Leaf: ?GetRenderPositionForThing@?A0x
// (Leaf is a prefix: the anonymous-namespace hash varies with the file path; it excludes the Call... wrapper.)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BBF00, length 180 bytes, `ret 4`: __fastcall (hidden C3DVector result
// in ecx, the thing in edx, the interpolation on the stack).
// Label: the catalog "Mesh_InterpolatePosition" is behavioural. Ego_r.pdb names the file-static
// `?GetRenderPositionForThing@?A0xc76c19b4@@YI?AVC3DVector@@ABVCThing@@M@Z`; the landed callers
// CTCGraphicAppearance::GetStaticFocalPosIfLoaded 0x004C11F0 (and 0x004C10E0 / 0x004BD2E0) call it with ecx = result, edx = thing.
// Callees: CTCPhysicsBase virtual slot 79 (+0x13C) GetRenderZOffset(float) const (CTCPhysicsBase 0x006B12F0 returns 0,
// CTCPhysicsControlled 0x00725E60 interpolates); float 1.0f @ 0x0122DED8.
// Model: the inline CTCPhysicsBase::GetPosition(float) = `t == 1 ? Position : (Position - OldPosition) * t + OldPosition`
// (value-constructor C3DVector operators as in the landed GetRHSet 0x00759B40; the selected vector's address in eax,
// then copied through the memberwise copy constructor); then
// `if (thing.ThingType == creature) pos.Z += PhysicsTC->GetRenderZOffset(t)`.
// Layout: CThing type byte +0x10, PhysicsTC +0x60 (retail; PDB +0x70 with 16-byte containers). engine/CTCPhysicsBase.h
// exists (Position +0x0C, OldPosition +0x18 agree) but flattens the C3DVectors to floats and has no virtual interface for
// slot 79, so the physics component is a local model with the same offsets.
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};

#define VSLOT(n) virtual void vslot##n();
#define VSLOT10(n) VSLOT(n##0) VSLOT(n##1) VSLOT(n##2) VSLOT(n##3) VSLOT(n##4) VSLOT(n##5) VSLOT(n##6) VSLOT(n##7) VSLOT(n##8) VSLOT(n##9)

class CTCPhysicsBase {
public:
    VSLOT10(0) VSLOT10(1) VSLOT10(2) VSLOT10(3) VSLOT10(4) VSLOT10(5) VSLOT10(6)
    VSLOT(70) VSLOT(71) VSLOT(72) VSLOT(73) VSLOT(74) VSLOT(75) VSLOT(76) VSLOT(77) VSLOT(78)
    virtual float GetRenderZOffset(float t) const;   // slot 79 (+0x13C)
    void*         m_pThing;                          // +0x04 (CTCBase)
    long          m_unknown08;                       // +0x08
    C3DVector     Position;                          // +0x0C
    C3DVector     OldPosition;                       // +0x18
    __forceinline C3DVector GetPosition(float t) const
    {
        return t == 1.0f ? Position : (Position - OldPosition) * t + OldPosition;
    }
};

enum { THING_TYPE_CREATURE = 2 };

class CThing {
public:
    unsigned char   _pad00[0x10];
    unsigned char   ThingType;                       // +0x10
    unsigned char   _pad11[0x60 - 0x11];
    CTCPhysicsBase* PhysicsTC;                       // +0x60
};

namespace {
C3DVector __fastcall GetRenderPositionForThing(const CThing& thing, float interpolation)
{
    const CTCPhysicsBase* physics = thing.PhysicsTC;
    C3DVector pos = physics->GetPosition(interpolation);
    if (thing.ThingType == THING_TYPE_CREATURE)
        pos.Z += physics->GetRenderZOffset(interpolation);
    return pos;
}
}

C3DVector __fastcall CallGetRenderPositionForThing(const CThing& thing, float interpolation)
{
    return GetRenderPositionForThing(thing, interpolation);
}