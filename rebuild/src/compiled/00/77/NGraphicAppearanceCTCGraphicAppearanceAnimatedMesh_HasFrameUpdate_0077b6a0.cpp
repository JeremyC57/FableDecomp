// NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCGraphicAppearanceAnimatedMesh@NGraphicAppearance@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077B6A0, thiscall, `ret`.
// Identity: slot 8 of NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh's vtable 0x0126ACDC (defined here: the slot holds this body in 1 vtable(s), all of NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCGraphicAppearanceAnimatedMesh@NGraphicAppearance@@UBE_NXZ; `ret` matches the signature.
namespace NGraphicAppearance {
class CTCGraphicAppearanceAnimatedMesh {
public:
    virtual bool HasFrameUpdate() const;
};
bool CTCGraphicAppearanceAnimatedMesh::HasFrameUpdate() const
{
    return true;
}
}
