// NCameraModes::CScriptModuleBase::OnInitLeaf() -- does nothing
// Leaf: ?OnInitLeaf@CScriptModuleBase@NCameraModes@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005ED110, thiscall, `ret`.
// Identity: slot 8 of NCameraModes::CScriptModuleDollyThing's vtable 0x01278188 (defined here: the slot holds this body in 8 vtable(s), all of NCameraModes::CScriptModuleBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts OnInitLeaf(void) at this slot; Ego_r/FableWin name set: ?OnInitLeaf@CScriptModuleBase@NCameraModes@@MAEXXZ; `ret` matches the signature.
namespace NCameraModes {
class CScriptModuleBase {
public:
protected:
    virtual void OnInitLeaf();
};
void CScriptModuleBase::OnInitLeaf()
{
}
}
