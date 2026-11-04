// NThingShot::CTCShotDefinitionBase::GetShootAtPos() const -- the shot's aim point: return the stored TargetPos by value
// Leaf: ?GetShootAtPos@CTCShotDefinitionBase@NThingShot@@IBE?AVC3DVector@@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BEDD0, 24 bytes, thiscall with the hidden C3DVector return pointer (`ret 4`):
//   three dwords copied from this+0x51 / +0x55 / +0x59.
// Identity (FINDINGS chunk 471; sweep2): the callers are the shot definitions' InitMovement / OnSubShotTrigger
//   (CTCShotDefinitionProjectile 0x007BBB3B, ...Predictive 0x007BC6FA, DragonFireball 0x007BCC40, MultiArrow 0x007BCF7F,
//   Throw 0x007BD2D1). Ego_r.pdb's name set gives CTCShotDefinitionBase the protected const accessor pair
//   `?GetShootAtPos@CTCShotDefinitionBase@NThingShot@@IBE?AVC3DVector@@XZ` / SetShootAtPos(C3DVector&). Its only
//   C3DVector-returning getter is that pair's getter, and the stored field is CTCShotDefinitionBase::TargetPos:
//   Ego_r +0x55 after the 1-byte SubShotsTriggered, minus 4 for the retail 12-byte HitThingsList vector = +0x51.
//   The landed name "CTCPhysicsRigidBody::GetFacingVector" is propagated.
// Layout: C3DVector is 1-aligned in retail (pack(1)), so TargetPos sits unaligned at +0x51; its memberwise copy
//   constructor gives the interleaved dword loads/stores (an implicit block copy goes through esi).
#pragma pack(push, 1)
#include "engine/C3DVector.h"
#pragma pack(pop)

namespace NThingShot {
#pragma pack(push, 1)
class CTCShotDefinitionBase {
public:
    unsigned char _pad00[0x51];                     // CTCBase, effect/target/shooter/weapon pointers, defs, floats,
                                                    //   StartFrame, SubShotsTriggered (+0x50)
    C3DVector     TargetPos;                        // +0x51
protected:
    C3DVector GetShootAtPos() const;
};
#pragma pack(pop)

C3DVector CTCShotDefinitionBase::GetShootAtPos() const
{
    return TargetPos;
}
}
