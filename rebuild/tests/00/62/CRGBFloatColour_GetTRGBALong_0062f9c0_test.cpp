#include <cstdio>
// Behaviour test: CRGBFloatColour::GetTRGBALong packs A, R, G, B (each * 255, truncated) high to low.
struct CRGBFloatColour {
    float R, G, B, A;
    unsigned long GetTRGBALong() const
    {
        return (((((long)(A * 255.0f) << 8) | (long)(R * 255.0f)) << 8 | (long)(G * 255.0f)) << 8) | (long)(B * 255.0f);
    }
};
int main()
{
    CRGBFloatColour c1 = { 1.0f, 0.5f, 0.0f, 1.0f };
    if (c1.GetTRGBALong() != 0xFFFF7F00UL) { std::printf("0062f9c0_TEST FAIL\n"); return 1; }
    CRGBFloatColour c2 = { 0.0f, 0.0f, 1.0f, 0.0f };
    if (c2.GetTRGBALong() != 0x000000FFUL) { std::printf("0062f9c0_TEST FAIL\n"); return 1; }
    CRGBFloatColour c3 = { 0.2f, 0.4f, 0.6f, 0.8f };
    if (c3.GetTRGBALong() != ((204UL << 24) | (51UL << 16) | (102UL << 8) | 153UL)) { std::printf("0062f9c0_TEST FAIL\n"); return 1; }
    std::printf("0062f9c0_TEST PASS\n");
    return 0;
}