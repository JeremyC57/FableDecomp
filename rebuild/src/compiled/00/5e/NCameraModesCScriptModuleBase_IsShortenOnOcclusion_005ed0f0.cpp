// NCameraModes::CScriptModuleBase::IsShortenOnOcclusion() const -- returns ShortenOnCollision
// Leaf: ?IsShortenOnOcclusion@CScriptModuleBase@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005ED0F0, thiscall, `ret`.
// Identity: slot 3 of NCameraModes::CScriptModuleDollyThing's vtable 0x01278188 (defined here: the slot holds this body in 11 vtable(s), all of NCameraModes::CScriptModuleBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts IsShortenOnOcclusion(void) at this slot; Ego_r/FableWin name set: ?IsShortenOnOcclusion@CScriptModuleBase@NCameraModes@@UBE_NXZ; `ret` matches the signature.
// Layout: Ego_r.pdb CScriptModuleBase has bool ShortenOnCollision at +0x41, the offset retail uses.
namespace NCameraModes {
class CScriptModuleBase {
public:
    virtual bool IsShortenOnOcclusion() const;
private:
    unsigned char _base_0x04[0x3D];   // +0x04 base class and earlier members (no shared header)
    bool ShortenOnCollision;   // +0x41 (Ego_r.pdb CScriptModuleBase)
};
bool CScriptModuleBase::IsShortenOnOcclusion() const
{
    return ShortenOnCollision;
}
}
