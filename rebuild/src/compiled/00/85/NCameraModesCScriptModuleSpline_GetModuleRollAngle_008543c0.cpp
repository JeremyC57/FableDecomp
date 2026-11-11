// NCameraModes::CScriptModuleSpline::GetModuleRollAngle(float&) const -- stores CurrentSplineRollAngle into the out value and returns true
// Leaf: ?GetModuleRollAngle@CScriptModuleSpline@NCameraModes@@UBE_NAAM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008543C0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 6 of NCameraModes::CScriptModuleSpline's vtable 0x01275A6C (the slot holds this body in 1 vtable(s), owner NCameraModes::CScriptModuleSpline); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts GetModuleRollAngle(float &) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetModuleRollAngle@CScriptModuleSpline@NCameraModes@@UBE_NAAM@Z; `ret` matches the signature.
// Was: NCameraModes::CScriptModuleSpline::vfunc_6 (rebuild/src/compiled/00/85/unattributed_vfunc6_008543c0.cpp).
// No shared headers for NCameraModes::CScriptModuleSpline / CScriptModuleBase: minimal local declaration of the real class (the
// base's 0x4c bytes as a pad), the Ego_r.pdb members at their offsets (unchanged in retail: the body reads +0x54).
namespace NCameraModes {
class CScriptModuleSpline {
public:
    virtual bool GetModuleRollAngle(float&) const;
protected:
    unsigned char _base_0x04[0x48];                                 // +0x04 CScriptModuleBase
    float Index;                                                    // +0x4c
    float CurrentSplineFOV;                                         // +0x50
    float CurrentSplineRollAngle;                                   // +0x54
};
bool CScriptModuleSpline::GetModuleRollAngle(float& angle) const
{
    angle = CurrentSplineRollAngle;
    return true;
}
}
