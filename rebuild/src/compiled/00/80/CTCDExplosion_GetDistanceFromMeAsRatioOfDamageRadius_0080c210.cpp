// CTCDExplosion::GetDistanceFromMeAsRatioOfDamageRadius(const CThing* thing) -- distance from the explosion's start-end
// Leaf: ?GetDistanceFromMeAsRatioOfDamageRadius@CTCDExplosion@@QBEMPBVCThing@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// segment to the thing, as a fraction of the damage radius
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080C210, length 31 bytes.
// Callees: CThing::GetPos() const @ 0x004C73D0, the __fastcall Geometry_DistancePointToLineSegment @ 0x008F5BA0
// (segment start in ecx, segment end in edx, the point on the stack; result in st0).
// Shape: VC evaluates the arguments right to left, so the thing's position is fetched first (`mov ecx,[esp+8]`, its
// reference pushed), then the two member addresses go to edx/ecx; the float result is divided in place by DamageRadius
// (`fdiv [esi+0Ch]`) and returned in st0.
// Layout: CTCDExplosion (Ego_r.pdb) -- DamageRadius +0x0c, StartPos +0x20, EndPos +0x2c; no engine header, local model.
#include "engine/C3DVector.h"

#include "engine/CThing.h"

// Naming (NF439 gen, 2026-11-25): callee 0x008F5BA0 binds its PDB spelling ?GetPerpendicularDistanceFromFiniteLine@NPhysicsHelpers@@YIMABVC3DVector@@00@Z (T1 target; was the free stand-in ?Geometry_DistancePointToLineSegment@@YIMABVC3DVector@@00@Z).
namespace NPhysicsHelpers { float __fastcall GetPerpendicularDistanceFromFiniteLine(C3DVector const& lineStart, C3DVector const& lineEnd, C3DVector const& point); }

class CTCDExplosion { public:
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
    return NPhysicsHelpers::GetPerpendicularDistanceFromFiniteLine(StartPos, EndPos, thing->GetPos()) / DamageRadius;
}