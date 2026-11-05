// CThing::SetAsRenderedThisFrame(unsigned long reasons) -- marks the thing's graphic appearance component as changed (CTCGraphicAppearance::SetAsChanged) when it has one
// Leaf: ?SetAsRenderedThisFrame@CThing@@QAEXK@Z
// Naming wave 31 (family srtf): own row is ?SetAsRenderedThisFrame@CThing@@QAEXK@Z (Ego_r.pdb block order + FableWin.pdb; was the descriptive
//   ?SetGraphicAppearanceChanged@CThing@@QAEXK@Z)
// Retail: Fable.exe @ 0x004C7590, 15 bytes, `ret 4` on the null path, tail jump otherwise. Catalog label
// CRenderTarget::DetachTarget is wrong: it sits in the CThing TU (0x004C73D0 GetPosition, 0x004C7770 SetAsActivated) and
// tail-jumps to CTCGraphicAppearance::SetAsChanged(unsigned long) 0x004BF9E0 on the member at +0x64; its callers
// (CTCLight::SetColour 0x00640C26.., PositionMesh 0x005EADA7) pass change flags. Name: Ego_r.pdb
// SetAsRenderedThisFrame 0x004C9150 sits between SetCurrentMapNumber 0x004C9140 (= retail 0x004C7580) and ClearRenderedThisFrame
// 0x004C9160 (= retail 0x004C75A0, a bare `ret`), at the 0x1BC0 delta of SetAsActivated; FableWin.pdb has the same symbol.
// Layout (retail): CThing::PTCGraphicAppearance +0x64 [Ego_r.pdb +0x74; retail CThing sits 0x10 below the PDB there,
// as in landed sources], so CThing is modelled locally.
namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    void SetAsChanged(unsigned long flags);              // 0x004BF9E0
};
}
using NGraphicAppearance::CTCGraphicAppearance;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"

void CThing::SetAsRenderedThisFrame(unsigned long reasons)
{
    if (PTCGraphicAppearance)
        PTCGraphicAppearance->SetAsChanged(reasons);
}