// NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh::FrameUpdate() -- does nothing
// Leaf: ?FrameUpdate@CTCGraphicAppearanceAnimatedMesh@NGraphicAppearance@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077B230, thiscall, `ret`.
// Identity: slot 9 of NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh's vtable 0x0126ACDC (defined here: the slot holds this body in 1 vtable(s), all of NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts FrameUpdate(void) at this slot; Ego_r/FableWin name set: ?FrameUpdate@CTCGraphicAppearanceAnimatedMesh@NGraphicAppearance@@UAEXXZ; `ret` matches the signature.
namespace NGraphicAppearance {
class CTCGraphicAppearanceAnimatedMesh {
public:
    virtual void FrameUpdate();
};
void CTCGraphicAppearanceAnimatedMesh::FrameUpdate()
{
}
}
