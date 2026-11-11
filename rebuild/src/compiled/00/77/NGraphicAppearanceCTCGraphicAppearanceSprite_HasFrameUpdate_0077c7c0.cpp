// NGraphicAppearance::CTCGraphicAppearanceSprite::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCGraphicAppearanceSprite@NGraphicAppearance@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077C7C0, thiscall, `ret`.
// Identity: slot 8 of NGraphicAppearance::CTCGraphicAppearanceSprite's vtable 0x0126AE94 (defined here: the slot holds this body in 1 vtable(s), all of NGraphicAppearance::CTCGraphicAppearanceSprite or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCGraphicAppearanceSprite@NGraphicAppearance@@UBE_NXZ; `ret` matches the signature.
namespace NGraphicAppearance {
class CTCGraphicAppearanceSprite {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCGraphicAppearanceSprite::HasFrameUpdate() const
{
    return true;
}
}
