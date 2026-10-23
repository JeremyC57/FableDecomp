// Grade: DIFFER(3v1395) -- parked x87-heavy row over 1 KB (skeleton only): x87-heavy (133 x87 ops) /Os frame `lea ebp,[esp-0x74]`, 0x358 locals, spirit health bars.
// CDrawSpiritsHealth::Draw() -- PARK SKELETON (decoded only)
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064E2CC, length 1395 bytes.
// Decode: x87-heavy (133 x87 ops) /Os frame `lea ebp,[esp-0x74]`, 0x358 locals, spirit health bars.
#pragma optimize("s", on)
struct CDrawSpiritsHealth {
    unsigned char _pad[0x100];
    void Draw(void* context);
};

void CDrawSpiritsHealth::Draw(void* context)
{
}
