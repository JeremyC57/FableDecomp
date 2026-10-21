// CThing::SetGraphicAppearanceChanged(unsigned long flags) -- marks the thing's graphic appearance component as changed (CTCGraphicAppearance::SetAsChanged) when it has one
// Retail: Fable.exe @ 0x004C7590, 15 bytes, `ret 4` on the null path, tail jump otherwise. Catalog label
// CRenderTarget::DetachTarget is wrong: it sits in the CThing TU (0x004C73D0 GetPosition, 0x004C7770 SetAsActivated) and
// tail-jumps to CTCGraphicAppearance::SetAsChanged(unsigned long) 0x004BF9E0 on the member at +0x64; its callers
// (CTCLight::SetColour 0x00640C26.., PositionMesh 0x005EADA7) pass change flags. The name is descriptive.
// Layout (retail): CThing::PTCGraphicAppearance +0x64 [Ego_r.pdb +0x74; retail CThing sits 0x10 below the PDB there,
// as in landed sources], so CThing is modelled locally.
class CTCGraphicAppearance {
public:
    void SetAsChanged(unsigned long flags);              // 0x004BF9E0
};
class CThing {
public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;          // +0x64
    void SetGraphicAppearanceChanged(unsigned long flags);
};

void CThing::SetGraphicAppearanceChanged(unsigned long flags)
{
    if (PTCGraphicAppearance)
        PTCGraphicAppearance->SetAsChanged(flags);
}