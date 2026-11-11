// NCameraModes::CScriptModuleCameraOnAStick::ResetCameraOnEnd() const -- returns false
// Leaf: ?ResetCameraOnEnd@CScriptModuleCameraOnAStick@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AB4A0, thiscall, `ret`.
// Identity: slot 7 of NCameraModes::CScriptModuleCameraOnAStick's vtable 0x0126CF04 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CScriptModuleCameraOnAStick or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 4 TLC-named slots of that vtable puts ResetCameraOnEnd(void) at this slot; Ego_r/FableWin name set: ?ResetCameraOnEnd@CScriptModuleCameraOnAStick@NCameraModes@@UBE_NXZ; `ret` matches the signature.
namespace NCameraModes {
class CScriptModuleCameraOnAStick {
public:
    virtual bool ResetCameraOnEnd() const;
};
bool CScriptModuleCameraOnAStick::ResetCameraOnEnd() const
{
    return false;
}
}
