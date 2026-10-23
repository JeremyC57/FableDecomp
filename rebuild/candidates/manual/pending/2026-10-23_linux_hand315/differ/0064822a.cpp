// Grade: DIFFER(3v1526) -- parked x87-heavy row over 1 KB (skeleton only): x87-heavy (140 x87 ops) /Os ebp-frame 0x26c, multiplier meter layout with float scaling and sprite draws.
// CDrawCombatMultiplier::Draw() -- PARK SKELETON (decoded only)
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064822A, length 1526 bytes.
// Decode: x87-heavy (140 x87 ops) /Os ebp-frame 0x26c, multiplier meter layout with float scaling and sprite draws.
#pragma optimize("s", on)
struct CDrawCombatMultiplier {
    unsigned char _pad[0x100];
    void Draw(void* context);
};

void CDrawCombatMultiplier::Draw(void* context)
{
}
