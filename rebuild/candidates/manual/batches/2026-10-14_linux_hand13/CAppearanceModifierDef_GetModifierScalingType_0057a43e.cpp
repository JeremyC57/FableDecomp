#pragma optimize("s", on)
// CAppearanceModifierDef::GetModifierScalingType() -- a modifier that is not part of a clothing suit maps the single
// body-area flag it covers to a scaling type (0x10 -> 1, 0x80 -> 2, 0x100 -> 3); anything else scales as type 0.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A43E, length 41 bytes. No callees.
// Layout: the retail CAppearanceModifierDef is the PDB layout minus 8 (the HideMaterials vector at +0x28 and the
// vector inside MorphGraphics are 12 bytes each, not 16): CoversBodyAreaFlags +0x40 (PDB 72), SuitPart +0x50
// (PDB 84). Size-optimised TU (`push 3 ; pop eax`); the sparse three-case switch lowers to the subtract chain.
struct CAppearanceModifierDef {
    unsigned char _pad0[0x40];
    long CoversBodyAreaFlags;                            // +0x40
    unsigned char _pad1[0xc];
    long SuitPart;                                       // +0x50 EClothingSuitPart
    long GetModifierScalingType() const;
};

long CAppearanceModifierDef::GetModifierScalingType() const
{
    if (SuitPart == 0) {
        switch (CoversBodyAreaFlags) {
        case 0x10:  return 1;
        case 0x80:  return 2;
        case 0x100: return 3;
        }
    }
    return 0;
}
