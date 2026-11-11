// NGraphicAppearance::CTCGraphicAppearanceStaticMesh::OnInitialActivate() -- does nothing
// Leaf: ?OnInitialActivate@CTCGraphicAppearanceStaticMesh@NGraphicAppearance@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077BA30, thiscall, `ret`.
// Identity: slot 6 of NGraphicAppearance::CTCGraphicAppearanceStaticMesh's vtable 0x0126AD6C (defined here: the slot holds this body in 1 vtable(s), all of NGraphicAppearance::CTCGraphicAppearanceStaticMesh or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnInitialActivate(void) at this slot; Ego_r/FableWin name set: ?OnInitialActivate@CTCGraphicAppearanceStaticMesh@NGraphicAppearance@@UAEXXZ; `ret` matches the signature.
namespace NGraphicAppearance {
class CTCGraphicAppearanceStaticMesh {
public:
    virtual void OnInitialActivate();
};
void CTCGraphicAppearanceStaticMesh::OnInitialActivate()
{
}
}
