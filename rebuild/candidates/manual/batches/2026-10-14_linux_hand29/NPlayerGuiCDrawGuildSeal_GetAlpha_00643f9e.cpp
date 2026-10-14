#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawGuildSeal::GetAlpha() const -- while drawing, one minus the flash alpha byte scaled to 0..1; otherwise fully opaque
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643F9E, length 42 bytes.
// Callees: none. Frame-pointer size TU with one 4-byte local: the byte is widened (`movzx`) into the local and loaded with `fild` (the unsigned char to
// float conversion), multiplied by the 1/255 constant (0x01231724) and subtracted from 1.0 (0x0122DED8, `fsubr`); the other arm loads 1.0. PDB
// CDrawGuildSeal: Drawing (CDrawBase) +0x18, FlashAlpha +0x2e -> retail +0x2a.
namespace NPlayerGui {

struct CDrawGuildSeal {
    unsigned char _pad[0x18];
    bool          Drawing;                         // +0x18
    unsigned char _pad19[0x11];
    unsigned char FlashAlpha;                      // +0x2a
    float GetAlpha() const;
};

float CDrawGuildSeal::GetAlpha() const
{
    if (Drawing)
        return 1.0f - FlashAlpha * (1.0f / 255.0f);
    return 1.0f;
}

}
