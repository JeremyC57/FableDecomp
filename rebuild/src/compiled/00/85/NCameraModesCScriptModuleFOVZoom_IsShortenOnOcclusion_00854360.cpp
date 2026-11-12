// NCameraModes::CScriptModuleFOVZoom::IsShortenOnOcclusion() const -- returns ShortenOnOcclusion
// Leaf: ?IsShortenOnOcclusion@CScriptModuleFOVZoom@NCameraModes@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00854360, thiscall, `ret`.
// Identity: slot 3 of NCameraModes::CScriptModuleFOVZoomStayRelative's vtable 0x01275A94 (defined here: the slot holds this body in 2 vtable(s), all of NCameraModes::CScriptModuleFOVZoom or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 5 TLC-named slots of that vtable puts IsShortenOnOcclusion(void) at this slot; Ego_r/FableWin name set: ?IsShortenOnOcclusion@CScriptModuleFOVZoom@NCameraModes@@UBE_NXZ; `ret` matches the signature.
// Layout: Ego_r.pdb CScriptModuleFOVZoom has bool ShortenOnOcclusion at +0x80, the offset retail uses.
namespace NCameraModes {
class CScriptModuleFOVZoom {
public:
    virtual bool IsShortenOnOcclusion() const;
private:
    unsigned char _base_0x04[0x7C];   // +0x04 base class and earlier members (no shared header)
    bool ShortenOnOcclusion;   // +0x80 (Ego_r.pdb CScriptModuleFOVZoom)
};
bool CScriptModuleFOVZoom::IsShortenOnOcclusion() const
{
    return ShortenOnOcclusion;
}
}
