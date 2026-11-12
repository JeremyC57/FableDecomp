// NCameraModes::CScriptModuleOpenChest::IsShortenOnOcclusion() const -- returns true
// Leaf: ?IsShortenOnOcclusion@CScriptModuleOpenChest@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008544F0, thiscall, `ret`.
// Identity: slot 3 of NCameraModes::CScriptModuleOpenChest's vtable 0x01275ABC (defined here: the slot holds this body in 1 vtable(s), all of NCameraModes::CScriptModuleOpenChest or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots of that vtable puts IsShortenOnOcclusion(void) at this slot; Ego_r/FableWin name set: ?IsShortenOnOcclusion@CScriptModuleOpenChest@NCameraModes@@UBE_NXZ; `ret` matches the signature.
namespace NCameraModes {
class CScriptModuleOpenChest {
public:
    virtual bool IsShortenOnOcclusion() const;
};
bool CScriptModuleOpenChest::IsShortenOnOcclusion() const
{
    return true;
}
}
