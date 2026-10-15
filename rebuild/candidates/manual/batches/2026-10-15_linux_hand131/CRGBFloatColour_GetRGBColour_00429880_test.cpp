// Behaviour test for 0x00429880 CRGBFloatColour::GetRGBColour: each float component scaled by 255 and truncated into the
// B, G, R, A byte layout.
#include <cstdio>
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CRGBFloatColour {
    float R, G, B, A;
    CRGBColour GetRGBColour() const
    { return CRGBColour((unsigned char)(R * 255.0f), (unsigned char)(G * 255.0f), (unsigned char)(B * 255.0f), (unsigned char)(A * 255.0f)); }
};
int main()
{
    CRGBFloatColour f; f.R = 1.0f; f.G = 0.5f; f.B = 0.0f; f.A = 0.25f;
    CRGBColour c = f.GetRGBColour();
    if (c.R != 255 || c.G != 127 || c.B != 0 || c.A != 63) { std::printf("00429880_TEST FAIL\n"); return 1; }
    std::printf("00429880_TEST PASS\n");
    return 0;
}
