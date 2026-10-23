// Grade: DIFFER(3v1337) -- parked x87-heavy row over 1 KB (skeleton only): x87-heavy (102 x87 ops) /Os ebp-frame HUD layout; screen-space float maths around CGraphicDataBank/font calls.
// CDrawBettingHUD::Draw() -- PARK SKELETON (decoded only)
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00649952, length 1337 bytes.
// Decode: x87-heavy (102 x87 ops) /Os ebp-frame HUD layout; screen-space float maths around CGraphicDataBank/font calls.
#pragma optimize("s", on)
struct CDrawBettingHUD {
    unsigned char _pad[0x100];
    void Draw(void* context);
};

void CDrawBettingHUD::Draw(void* context)
{
}
