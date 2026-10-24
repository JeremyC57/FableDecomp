// NUISystem::CComponent::SetColour(const CRGBColour& colour) -- snap the colour: target, initial and current colour all take the argument
// Leaf: SetColour
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052E7A0, 27 bytes (`ret 4`), no callees.
// Label: the catalog "NUISystem::CSprite::vfunc_13" is the first vtable (0x0122F5D4 slot 13) listing this body;
// it is CComponent code (the colour twin of SetZoom slot 11 / SetPosition slot 15). Ego_r.pdb names
// ?SetColour@CComponent@NUISystem@@UAEXABVCRGBColour@@@Z.
// Layout: retail CComponent (PDB offsets minus 8 from Time on): Colour +0x84 (PDB +0x8c), TargetColour +0x88,
// InitialColour +0x8c. CRGBColour is one packed dword.
// Shape: the dword is read once into eax and stored target, initial, current: the argument is copied into a
// local first (written straight from the reference, VC re-reads it after each store because it may alias).
struct CRGBColour {
    unsigned long Colour;
};

namespace NUISystem {
struct CComponent {
    unsigned char _pad00[0x84];
    CRGBColour Colour;                      // +0x84
    CRGBColour TargetColour;                // +0x88
    CRGBColour InitialColour;               // +0x8c
    void SetColour(const CRGBColour& colour);
};

void CComponent::SetColour(const CRGBColour& colour)
{
    CRGBColour value = colour;
    TargetColour = value;
    InitialColour = value;
    Colour = value;
}
}