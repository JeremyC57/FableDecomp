// CTCDExplosion::GetDistanceFromMeAsRatioOfDamageRadius(const CThing* thing) -- distance from the explosion's start-end
// segment to the thing, as a fraction of the damage radius
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080C210, length 31 bytes.
// Callees: CThing::GetPosition() const @ 0x004C73D0, the __fastcall Geometry_DistancePointToLineSegment @ 0x008F5BA0
// (segment start in ecx, segment end in edx, the point on the stack; result in st0).
// Shape: VC evaluates the arguments right to left, so the thing's position is fetched first (`mov ecx,[esp+8]`, its
// reference pushed), then the two member addresses go to edx/ecx; the float result is divided in place by DamageRadius
// (`fdiv [esi+0Ch]`) and returned in st0.
// Layout: CTCDExplosion (Ego_r.pdb) -- DamageRadius +0x0c, StartPos +0x20, EndPos +0x2c; no engine header, local model.
struct C3DVector { float X, Y, Z; };

struct CThing {
    const C3DVector& GetPosition() const;   // 0x004C73D0
};

float __fastcall Geometry_DistancePointToLineSegment(const C3DVector& lineStart, const C3DVector& lineEnd,
                                                     const C3DVector& point);   // 0x008F5BA0

struct CTCDExplosion {
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    float DamageRadius;                     // +0x0c
    float PhysicsRadius;                    // +0x10
    float MaxDamage;                        // +0x14
    long FireDamage;                        // +0x18
    long ExplosionLifeTimer;                // +0x1c
    C3DVector StartPos;                     // +0x20
    C3DVector EndPos;                       // +0x2c
    float GetDistanceFromMeAsRatioOfDamageRadius(const CThing* thing) const;
};

float CTCDExplosion::GetDistanceFromMeAsRatioOfDamageRadius(const CThing* thing) const
{
    return Geometry_DistancePointToLineSegment(StartPos, EndPos, thing->GetPosition()) / DamageRadius;
}
