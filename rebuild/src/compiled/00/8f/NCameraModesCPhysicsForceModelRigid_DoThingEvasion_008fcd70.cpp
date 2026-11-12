// NCameraModes::CPhysicsForceModelRigid::DoThingEvasion() -- returns the zero vector (this force model applies no thing-evasion)
// Leaf: ?DoThingEvasion@CPhysicsForceModelRigid@NCameraModes@@UAE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FCD70, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 2 of NCameraModes::CPhysicsForceModelRigid's vtable 0x01276AC8 (the slot holds this body in 1 vtable(s), owner NCameraModes::CPhysicsForceModelRigid); the Xbox 360 Test PDB virtual order (third source) aligned on 1 TLC-named slots of that vtable puts DoThingEvasion(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?DoThingEvasion@CPhysicsForceModelRigid@NCameraModes@@UAE?AVC3DVector@@XZ; `ret` matches the signature.
// Was: NCameraModes::CPhysicsForceModelRigid::vfunc_2 (rebuild/src/compiled/00/8f/NCameraModesCPhysicsForceModelRigid_vfunc2_008fcd70.cpp).
// No shared header for NCameraModes::CPhysicsForceModelRigid (RTTI : NCameraModes::CPhysicsForceModelBase): minimal local declaration of the real class.
#define FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_NCAMERAMODES_CBASE_POSITION
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMECAMERAMANAGER_WORLD
#define FABLE_RETAIL_CPHYSICSCOLLISIONPOINTSOBJECT_POINTS
#include "engine/NCameraModes_CPhysicsForceModelBase.h"
namespace NCameraModes {
class CPhysicsForceModelRigid : public CPhysicsForceModelBase {
public:
    virtual C3DVector DoThingEvasion();
};
C3DVector CPhysicsForceModelRigid::DoThingEvasion()
{
    return C3DVector(0.0f, 0.0f, 0.0f);
}
}
