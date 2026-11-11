// NCameraModes::CScriptModuleBase::GetModuleRollAngle(float&) const -- returns false
// Leaf: ?GetModuleRollAngle@CScriptModuleBase@NCameraModes@@UBE_NAAM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005ED100, thiscall, `ret 4`.
// Identity: slot 6 of NCameraModes::CScriptModuleDollyThing's vtable 0x01278188 (defined here: the slot holds this body in 15 vtable(s), all of NCameraModes::CScriptModuleBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts GetModuleRollAngle(float &) at this slot; Ego_r/FableWin name set: ?GetModuleRollAngle@CScriptModuleBase@NCameraModes@@UBE_NAAM@Z; `ret` matches the signature.
namespace NCameraModes {
class CScriptModuleBase {
public:
    virtual bool GetModuleRollAngle(float&) const;
};
bool CScriptModuleBase::GetModuleRollAngle(float&) const
{
    return false;
}
}
