// Grade: DIFFER(3v1910) -- parked x87-heavy row over 1 KB (skeleton only): x87-heavy (191 x87 ops) /Os ebp-frame health bar + OBJECT_RESURRECTION_PHIAL counter, float fill ratios.
// CDrawHealth::Draw() -- PARK SKELETON (decoded only)
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00647AB4, length 1910 bytes.
// Decode: x87-heavy (191 x87 ops) /Os ebp-frame health bar + OBJECT_RESURRECTION_PHIAL counter, float fill ratios.
#pragma optimize("s", on)
struct CDrawHealth {
    unsigned char _pad[0x100];
    void Draw(void* context);
};

void CDrawHealth::Draw(void* context)
{
}
