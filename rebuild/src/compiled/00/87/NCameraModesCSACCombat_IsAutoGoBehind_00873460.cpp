// NCameraModes::CSACCombat::IsAutoGoBehind() const -- returns AutoGoBehind
// Leaf: ?IsAutoGoBehind@CSACCombat@NCameraModes@@MBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00873460, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 28 of NCameraModes::CSACCombat's vtable 0x012775B4 (the slot holds this body in 1 vtable(s), owner NCameraModes::CSACCombat); the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots of that vtable puts IsAutoGoBehind(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?IsAutoGoBehind@CSACCombat@NCameraModes@@MBE_NXZ; `ret` matches the signature.
// Was: NCameraModes::CSACCombat::vfunc_28 (rebuild/src/compiled/00/87/gapscan_sub00873460_00873460.cpp).
// No shared header for NCameraModes::CSACCombat (RTTI : CStringAndCageBase : CBase): minimal local declaration of the real class
// with its two Ego_r.pdb members (AutoGoBehind / AutoGoBehindTime, 0x24 lower in retail: CBase is 0xa4 there, 0xc8 in Ego_r).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#include "engine/NCameraModes_CBase.h"
#include "engine/NCameraModes_CStringAndCageBase.h"
namespace NCameraModes {
class CSACCombat : public CStringAndCageBase {
protected:
    virtual bool IsAutoGoBehind() const;
    virtual float GetAutoGoBehindTime() const;
private:
    bool AutoGoBehind;                                              // +0xc8
    float AutoGoBehindTime;                                         // +0xcc
};
bool CSACCombat::IsAutoGoBehind() const
{
    return AutoGoBehind;
}
}
