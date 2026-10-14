#include <cstdio>
#include <cstring>
static const bool g_is32 = sizeof(void*) == 4 && sizeof(long) == 4;   // the layout checks hold for the 32-bit VC7.1 build
struct CRGBColour { unsigned char B, G, R, A; void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); } };
struct CShadowColourState { unsigned char _pad[0x30]; bool m_enabled; unsigned char _pad31[3]; CRGBColour m_colour0; CRGBColour m_colour1; CRGBColour m_colour2; long m_value40; long m_value44; long m_value48;
    CShadowColourState() : m_colour0(1, 2, 3, 4), m_colour1(1, 2, 3, 4), m_colour2(1, 2, 3, 4) { m_enabled = true; m_value40 = m_value44 = m_value48 = 7; }
    void Reset() { m_enabled = false; m_colour0 = CRGBColour(0, 0, 0, 255); m_colour1 = CRGBColour(0, 0, 0, 255); m_colour2 = CRGBColour(0, 0, 0, 255); m_value40 = 0; m_value44 = 0; m_value48 = 0; } };
static bool IsBlack(const CRGBColour& c) { return c.R == 0 && c.G == 0 && c.B == 0 && c.A == 255; }
int main()
{
    CShadowColourState s; s.Reset();
    if (s.m_enabled || !IsBlack(s.m_colour0) || !IsBlack(s.m_colour1) || !IsBlack(s.m_colour2) || s.m_value40 || s.m_value44 || s.m_value48 || (g_is32 && (char*)&s.m_colour0 - (char*)&s != 0x34) || (g_is32 && (char*)&s.m_value48 - (char*)&s != 0x48)) { std::printf("00ac0250_TEST FAIL\n"); return 1; }
    std::printf("00ac0250_TEST PASS\n");
    return 0;
}