// CThingFilter_IsWithinArea::operator()(CThing* thing) -- true when the thing lies within Radius (plus its own physics radius when IncludeThingRad) of the segment StartPos..EndPos in the XY plane
// Leaf: ??RCThingFilter_IsWithinArea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FD830, length 116 bytes.
// Callees: CThing::GetPosition @ 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof" is a propagated label),
// GetLineOutFromClosestPointOnFiniteLineToPoint(const C3DVector&, const C3DVector&, const C3DVector&) @ 0x008F5B50
// (__fastcall returning C3DVector through a hidden pointer in ecx, so C3DVector is non-POD here).
// Layout: PDB CThingFilter_IsWithinArea { C3DVector* StartPos (+0); C3DVector* EndPos (+4); float Radius (+8); bool IncludeThingRad (+0xc); };
// retail CThing PhysicsTC +0x60, the physics component's radius float at +0x34 (local model).
// Shape: the thing radius is a float local zeroed first and overwritten when IncludeThingRad; the line-out vector
// is a named local; `out.X*out.X + out.Y*out.Y < r*r` (VC7.1 emits the Y term first) with the compare materialised as an int.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CTCPhysicsBase {
    unsigned char _pad00[0x34];
    float         m_radius;                     // +0x34
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
C3DVector __fastcall GetLineOutFromClosestPointOnFiniteLineToPoint(const C3DVector& start, const C3DVector& end, const C3DVector& point);   // 0x008F5B50
struct CThingFilter_IsWithinArea {
    C3DVector* StartPos;                        // +0x00
    C3DVector* EndPos;                          // +0x04
    float      Radius;                          // +0x08
    bool       IncludeThingRad;                 // +0x0c
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsWithinArea::operator()(CThing* thing) const
{
    float thingRadius = 0.0f;
    if (IncludeThingRad)
        thingRadius = thing->PhysicsTC->m_radius;
    C3DVector out = GetLineOutFromClosestPointOnFiniteLineToPoint(*StartPos, *EndPos, thing->GetPosition());
    float radius = thingRadius + Radius;
    return out.X * out.X + out.Y * out.Y < radius * radius;
}
