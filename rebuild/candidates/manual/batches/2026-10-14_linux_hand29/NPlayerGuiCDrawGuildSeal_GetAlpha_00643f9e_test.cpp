#include <cstdio>
namespace NPlayerGui {
struct CDrawGuildSeal { unsigned char _pad[0x18]; bool Drawing; unsigned char _pad19[0x11]; unsigned char FlashAlpha;
    float GetAlpha() const { if (Drawing) return 1.0f - FlashAlpha * (1.0f / 255.0f); return 1.0f; } }; }
int main() { NPlayerGui::CDrawGuildSeal s; s.Drawing = false; s.FlashAlpha = 255; if (s.GetAlpha() != 1.0f) { std::printf("00643f9e_TEST FAIL\n"); return 1; } s.Drawing = true; if (s.GetAlpha() > 0.001f || s.GetAlpha() < -0.001f) { std::printf("00643f9e_TEST FAIL\n"); return 1; }
    s.FlashAlpha = 0; if (s.GetAlpha() != 1.0f) { std::printf("00643f9e_TEST FAIL\n"); return 1; } std::printf("00643f9e_TEST PASS\n"); return 0; }
