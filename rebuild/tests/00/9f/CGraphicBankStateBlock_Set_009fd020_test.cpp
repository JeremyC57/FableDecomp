#include <cstdio>
#include <cstring>
// Host model of CGraphicBankStateBlock::Set: a field-by-field copy of the bank state.
struct PixelFormats { int f[7]; };
struct State {
    PixelFormats bank, compressed;
    int type, quality, minW, minH, maxW, maxH, sel, bump;
    bool sprite, multi, bumpAlpha, dxt1, reduce, compress, section, sign;
};
static void Set(State& d, const State& s)
{
    d.bank = s.bank; d.compressed = s.compressed; d.type = s.type; d.quality = s.quality;
    d.minW = s.minW; d.minH = s.minH; d.maxW = s.maxW; d.maxH = s.maxH; d.sprite = s.sprite;
    d.sel = s.sel; d.bump = s.bump; d.sprite = s.sprite; d.multi = s.multi; d.bumpAlpha = s.bumpAlpha;
    d.dxt1 = s.dxt1; d.reduce = s.reduce; d.compress = s.compress; d.section = s.section; d.sign = s.sign;
}
int main()
{
    State a, b;
    std::memset(&a, 0, sizeof(a)); std::memset(&b, 0, sizeof(b));
    for (int i = 0; i < 7; ++i) { a.bank.f[i] = i + 1; a.compressed.f[i] = 10 * i; }
    a.type = 2; a.quality = 80; a.minW = 4; a.minH = 8; a.maxW = 512; a.maxH = 256; a.sel = 3; a.bump = 7;
    a.sprite = true; a.multi = false; a.bumpAlpha = true; a.dxt1 = true; a.reduce = false; a.compress = true; a.section = true; a.sign = true;
    Set(b, a);
    if (std::memcmp(&a, &b, sizeof(a)) != 0) { std::printf("009fd020_TEST FAIL\n"); return 1; }
    std::printf("009fd020_TEST PASS\n");
    return 0;
}