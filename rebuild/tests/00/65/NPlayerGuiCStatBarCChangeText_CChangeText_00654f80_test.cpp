// Behaviour test for 0x00654F80 NPlayerGui::CStatBar::CChangeText ctor: both offsets from the argument, colour copied, not yet moved (standalone copy of the logic)
#include <cstdio>
struct Vec { float X, Y; };
struct Colour { unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
struct ChangeText { Vec OldOffset, Offset; const char* Text; long Value; Colour Col; bool StartToEnd; float PercentageThrough;
    ChangeText(const Vec& o, const Colour& c, const char* t, long v, bool s) : OldOffset(o), Offset(o), Text(t), Value(v), StartToEnd(s), PercentageThrough(0.0f) { Col.Set(c.R, c.G, c.B, c.A); }
    ChangeText(const ChangeText& x) : OldOffset(x.OldOffset), Offset(x.Offset), Text(x.Text), Value(x.Value), StartToEnd(x.StartToEnd), PercentageThrough(x.PercentageThrough) { Col.Set(x.Col.R, x.Col.G, x.Col.B, x.Col.A); } };

int main()
{
    Vec o; o.X = 3.0f; o.Y = 4.0f; Colour c; c.R = 1; c.G = 2; c.B = 3; c.A = 4;
    ChangeText t(o, c, "+5", 5, true);
    if (t.OldOffset.X != 3.0f || t.Offset.Y != 4.0f || t.Value != 5 || !t.StartToEnd || t.PercentageThrough != 0.0f) { std::printf("00654f80_TEST FAIL\n"); return 1; }
    if (t.Col.R != 1 || t.Col.G != 2 || t.Col.B != 3 || t.Col.A != 4) { std::printf("00654f80_TEST FAIL\n"); return 1; }
    std::printf("00654f80_TEST PASS\n");
    return 0;
}