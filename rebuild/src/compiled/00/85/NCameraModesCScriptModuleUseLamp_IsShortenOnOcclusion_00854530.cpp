// NCameraModes::CScriptModuleUseLamp::IsShortenOnOcclusion() const -- returns true
// Leaf: ?IsShortenOnOcclusion@CScriptModuleUseLamp@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00854530, thiscall, `ret`.
// Identity: slot 3 of NCameraModes::CScriptModuleUseLamp's vtable 0x01275AE4 (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CScriptModuleUseLamp or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts IsShortenOnOcclusion(void) at this slot; Ego_r/FableWin name set: ?IsShortenOnOcclusion@CScriptModuleUseLamp@NCameraModes@@UBE_NXZ; `ret` matches the signature.
namespace NCameraModes {
class CScriptModuleUseLamp {
public:
    virtual bool IsShortenOnOcclusion() const;
};
bool CScriptModuleUseLamp::IsShortenOnOcclusion() const
{
    return true;
}
}
