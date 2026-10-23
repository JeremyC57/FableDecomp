// Grade: DIFFER(3v1343) -- parked x87-heavy row over 1 KB (skeleton only): x87-heavy (100 x87 ops, two ftol 0xBFEA70) /Os frame `lea ebp,[esp-0x74]`, targeting reticle projection.
// CDrawProjectileTargetingCentre::Draw() -- PARK SKELETON (decoded only)
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00647575, length 1343 bytes.
// Decode: x87-heavy (100 x87 ops, two ftol 0xBFEA70) /Os frame `lea ebp,[esp-0x74]`, targeting reticle projection.
#pragma optimize("s", on)
struct CDrawProjectileTargetingCentre {
    unsigned char _pad[0x100];
    void Draw(void* context);
};

void CDrawProjectileTargetingCentre::Draw(void* context)
{
}
