// Behaviour test for NParticleEngine::CPSCRenderMesh::InterpolateTo (0x00AFF1B0): a standalone model of the blend -- index members
// (mesh / trail bank, blend modes and ops) and RenderSizeParam take the other component's value only when t > 0.5; the packed
// integers and the bool flags blend as round((1 - t) * a + b * t); colours blend per channel at weight round(t * 256).
#include <cstdio>
#include <cmath>
static unsigned long Interp(unsigned long a, unsigned long b, float t) { return (unsigned long)std::floor((1.0f - t) * a + b * t + 0.5f); }
static unsigned long InterpColour(unsigned long a, unsigned long b, float t) {
    unsigned long w = (unsigned long)std::floor(t * 256.0f + 0.5f), r = 0;
    for (int s = 0; s < 32; s += 8) { unsigned long ca = (a >> s) & 255, cb = (b >> s) & 255; r |= ((ca * (256 - w) + cb * w) >> 8) << s; }
    return r;
}
struct Mesh {
    long BankIndex, TrailBankIndex; unsigned long StartColour, BlendMode, StartRenderSizeX, FadeOutBeginInteger, CentredOnPos, RenderSizeParam;
    void InterpolateTo(const Mesh& o, float t) {
        BankIndex = t > 0.5f ? o.BankIndex : BankIndex;
        StartRenderSizeX = Interp(StartRenderSizeX, o.StartRenderSizeX, t);
        RenderSizeParam = t > 0.5f ? o.RenderSizeParam : RenderSizeParam;
        StartColour = InterpColour(StartColour, o.StartColour, t);
        CentredOnPos = Interp(CentredOnPos, o.CentredOnPos, t);
        FadeOutBeginInteger = Interp(FadeOutBeginInteger, o.FadeOutBeginInteger, t);
        TrailBankIndex = t > 0.5f ? o.TrailBankIndex : TrailBankIndex;
        BlendMode = t > 0.5f ? o.BlendMode : BlendMode;
    }
};
int main()
{
    Mesh a = { 3, 7, 0x00000000, 1, 100, 1000, 0, 11 };
    Mesh b = { 9, 4, 0xFF80FF00, 2, 300, 0, 1, 22 };
    Mesh c = a; c.InterpolateTo(b, 0.25f);
    bool ok = c.BankIndex == 3 && c.TrailBankIndex == 7 && c.BlendMode == 1 && c.StartRenderSizeX == 150 && c.FadeOutBeginInteger == 750
        && c.CentredOnPos == 0 && c.RenderSizeParam == 11 && c.StartColour == 0x3F203F00;
    Mesh d = a; d.InterpolateTo(b, 0.75f);
    ok = ok && d.BankIndex == 9 && d.TrailBankIndex == 4 && d.BlendMode == 2 && d.StartRenderSizeX == 250 && d.CentredOnPos == 1
        && d.RenderSizeParam == 22 && d.FadeOutBeginInteger == 250;
    printf(ok ? "00aff1b0_TEST PASS\n" : "00aff1b0_TEST FAIL\n");
    return ok ? 0 : 1;
}