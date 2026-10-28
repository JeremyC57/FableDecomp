// behaviour test for 0x004BD040 CTCGraphicAppearance::GetTintColour (channel-wise average of the tint map, white when empty)
#include <cstdio>
#include <map>
struct RGBA { unsigned char B, G, R, A; };
static RGBA make(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { RGBA c; c.R = r; c.G = g; c.B = b; c.A = a; return c; }
static RGBA GetTintColour(const std::map<void*, RGBA>& tints)
{
    if (tints.empty()) return make(255, 255, 255, 255);
    float a = 0.0f, b = 0.0f, g = 0.0f, r = 0.0f;
    for (std::map<void*, RGBA>::const_iterator it = tints.begin(); it != tints.end(); ++it) {
        r += it->second.R; g += it->second.G; b += it->second.B; a += it->second.A;
    }
    float count = (float)tints.size();
    return make((unsigned char)(r / count), (unsigned char)(g / count), (unsigned char)(b / count), (unsigned char)(a / count));
}
int main()
{
    std::map<void*, RGBA> m;
    RGBA w = GetTintColour(m);
    bool ok = w.R == 255 && w.G == 255 && w.B == 255 && w.A == 255;
    int k1, k2, k3;
    m[&k1] = make(10, 200, 0, 255); m[&k2] = make(20, 100, 3, 0); m[&k3] = make(31, 0, 4, 128);
    RGBA c = GetTintColour(m);
    ok = ok && c.R == 20 && c.G == 100 && c.B == 2 && c.A == 127;   // truncating averages
    std::printf(ok ? "004bd040_TEST PASS\n" : "004bd040_TEST FAIL\n");
    return ok ? 0 : 1;
}