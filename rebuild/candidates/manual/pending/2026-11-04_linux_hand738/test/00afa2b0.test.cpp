// Behaviour test for NParticleEngine::CPSCRenderSprite::ReadBinary (0x00AFA2B0): a standalone model of the read order -- two SLONG
// bank indices, three colours, the 19 packed settings as ULONGs (each stored into its bit-field, so only the low bits survive) in
// BlendMode, TrailBlendMode, BlendOp, TrailBlendOp, Flags, FadeInEnd, FadeOutBegin, TrailLength, FlickerMinAlpha, FlickerMinSize,
// NoCrossedSprites, StartRenderSize, AlphaFadeMinimum, EndRenderSize, FlickerBias, AnimationTimeSecs, SizeFadeMinimum, TrailWidth,
// FlickerSpeed order, then the seven flags as EBOOLs (UseStart/Mid/EndColour, AlphaFade, SizeFade, Flicker, ForceAnimationTime).
#include <cstdio>
struct Stream { const unsigned long* p; unsigned long Read() { return *p++; } };
static const int Widths[19] = { 3, 3, 3, 3, 10, 10, 10, 12, 10, 10, 5, 11, 7, 11, 8, 14, 7, 10, 12 };
struct Sprite {
    long Sprite_, Trail; unsigned long Colours[3], Packed[19]; bool Flags[7];
    void ReadBinary(Stream& in)
    {
        Sprite_ = (long)in.Read(); Trail = (long)in.Read();
        for (int i = 0; i < 3; ++i) Colours[i] = in.Read();
        for (int i = 0; i < 19; ++i) Packed[i] = in.Read() & ((1ul << Widths[i]) - 1);
        for (int i = 0; i < 7; ++i) Flags[i] = in.Read() != 0;
    }
};
int main()
{
    unsigned long data[2 + 3 + 19 + 7];
    for (int i = 0; i < 31; ++i) data[i] = 0x1000u + i * 0x111u;
    data[0] = (unsigned long)-5; data[24] = 0; data[25] = 1; data[26] = 2;
    Stream s = { data }; Sprite sp; sp.ReadBinary(s);
    bool ok = sp.Sprite_ == -5 && sp.Trail == (long)data[1] && sp.Colours[2] == data[4] && s.p == data + 31;
    for (int i = 0; i < 19; ++i) ok = ok && sp.Packed[i] == (data[5 + i] & ((1ul << Widths[i]) - 1));
    ok = ok && !sp.Flags[0] && sp.Flags[1] && sp.Flags[2] && sp.Packed[0] == ((0x1000u + 5 * 0x111u) & 7);
    printf(ok ? "00afa2b0_TEST PASS\n" : "00afa2b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
