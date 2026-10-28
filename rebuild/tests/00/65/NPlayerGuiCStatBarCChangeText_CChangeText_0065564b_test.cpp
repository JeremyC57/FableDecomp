// Behaviour test for 0x0065564B NPlayerGui::CStatBar::CChangeText copy ctor: member-wise copy (standalone copy of the logic)
#include <cstdio>
struct Vec { float X, Y; };
struct Colour { unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
struct ChangeText { Vec OldOffset, Offset; const char* Text; long Value; Colour Col; bool StartToEnd; float PercentageThrough;
    ChangeText(const Vec& o, const Colour& c, const char* t, long v, bool s) : OldOffset(o), Offset(o), Text(t), Value(v), StartToEnd(s), PercentageThrough(0.0f) { Col.Set(c.R, c.G, c.B, c.A); }
    ChangeText(const ChangeText& x) : OldOffset(x.OldOffset), Offset(x.Offset), Text(x.Text), Value(x.Value), StartToEnd(x.StartToEnd), PercentageThrough(x.PercentageThrough) { Col.Set(x.Col.R, x.Col.G, x.Col.B, x.Col.A); } };

int main()
{
    Vec o; o.X = 1.0f; o.Y = 2.0f; Colour c; c.R = 9; c.G = 8; c.B = 7; c.A = 6;
    ChangeText a(o, c, "-1", -1, false); a.PercentageThrough = 0.5f; a.Offset.X = 10.0f;
    ChangeText b(a);
    if (b.Offset.X != 10.0f || b.OldOffset.X != 1.0f || b.Value != -1 || b.StartToEnd || b.PercentageThrough != 0.5f || b.Text != a.Text) { std::printf("0065564b_TEST FAIL\n"); return 1; }
    if (b.Col.R != 9 || b.Col.G != 8 || b.Col.B != 7 || b.Col.A != 6) { std::printf("0065564b_TEST FAIL\n"); return 1; }
    std::printf("0065564b_TEST PASS\n");
    return 0;
}