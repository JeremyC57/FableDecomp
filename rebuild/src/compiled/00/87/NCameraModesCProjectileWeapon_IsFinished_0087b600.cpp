// NCameraModes::CProjectileWeapon::IsFinished() const -- returns Finished
// Leaf: ?IsFinished@CProjectileWeapon@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087B600, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 13 of NCameraModes::CProjectileWeapon's vtable 0x01277804 (the slot holds this body in 1 vtable(s), owner NCameraModes::CProjectileWeapon); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts IsFinished(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsFinished@CProjectileWeapon@NCameraModes@@UBE_NXZ; `ret` matches the signature.
// Was: NCameraModes::CProjectileWeapon::vfunc_13 (rebuild/src/compiled/00/87/gapscan_sub0087b600_0087b600.cpp).
// No shared header for NCameraModes::CProjectileWeapon (RTTI : CBase): minimal local declaration with the Ego_r.pdb members 0x24
// lower in retail.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#include "engine/NCameraModes_CBase.h"
namespace NCameraModes {
class CProjectileWeapon : public CBase {
public:
    virtual bool IsFinished() const;
protected:
    long ShotFiredTimer;                                            // +0xa4
    bool Finished;                                                  // +0xa8
};
bool CProjectileWeapon::IsFinished() const
{
    return Finished;
}
}
