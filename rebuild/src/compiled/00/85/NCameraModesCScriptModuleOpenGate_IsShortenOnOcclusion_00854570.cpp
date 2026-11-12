// NCameraModes::CScriptModuleOpenGate::IsShortenOnOcclusion() const -- returns true
// Leaf: ?IsShortenOnOcclusion@CScriptModuleOpenGate@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00854570, thiscall, `ret`.
// Identity: slot 3 of NCameraModes::CScriptModuleOpenGate's vtable 0x01275B0C (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CScriptModuleOpenGate or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts IsShortenOnOcclusion(void) at this slot; Ego_r/FableWin name set: ?IsShortenOnOcclusion@CScriptModuleOpenGate@NCameraModes@@UBE_NXZ; `ret` matches the signature.
namespace NCameraModes {
class CScriptModuleOpenGate {
public:
    virtual bool IsShortenOnOcclusion() const;
};
bool CScriptModuleOpenGate::IsShortenOnOcclusion() const
{
    return true;
}
}
