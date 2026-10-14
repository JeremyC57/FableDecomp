#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CRGBAColour { float R, G, B, A; CRGBAColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { } };
struct CIEngine { CRGBAColour PeekGlobalAmbientColour() { return CRGBAColour(1.0f, 1.0f, 1.0f, 1.0f); } };

int main() {
    CIEngine e; CRGBAColour c = e.PeekGlobalAmbientColour(); if (c.R != 1.0f || c.G != 1.0f || c.B != 1.0f || c.A != 1.0f) { std::printf("00b28b60_TEST FAIL\n"); return 1; }
    std::printf("00b28b60_TEST PASS\n"); return 0;
}
