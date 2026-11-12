// NCameraModes::CScriptModuleFOVZoom::ResetCameraOnEnd() const -- returns ResetCamera
// Leaf: ?ResetCameraOnEnd@CScriptModuleFOVZoom@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00854370, thiscall, `ret`.
// Identity: slot 7 of NCameraModes::CScriptModuleFOVZoomStayRelative's vtable 0x01275A94 (defined here: the slot holds this body in 2 vtable(s), all of NCameraModes::CScriptModuleFOVZoom or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts ResetCameraOnEnd(void) at this slot; Ego_r/FableWin name set: ?ResetCameraOnEnd@CScriptModuleFOVZoom@NCameraModes@@UBE_NXZ; `ret` matches the signature.
// Layout: Ego_r.pdb CScriptModuleFOVZoom has bool ResetCamera at +0x81, the offset retail uses.
namespace NCameraModes {
class CScriptModuleFOVZoom {
public:
    virtual bool ResetCameraOnEnd() const;
private:
    unsigned char _base_0x04[0x7D];   // +0x04 base class and earlier members (no shared header)
    bool ResetCamera;   // +0x81 (Ego_r.pdb CScriptModuleFOVZoom)
};
bool CScriptModuleFOVZoom::ResetCameraOnEnd() const
{
    return ResetCamera;
}
}
