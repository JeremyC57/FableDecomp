#include <cstdio>
// Behaviour model of CTCLight::SetColour (0x00640BB0).
struct Col { unsigned char B, G, R, A; };
static bool Eq(const Col& a, const Col& b) { return a.R == b.R && a.G == b.G && a.B == b.B && a.A == b.A; }
struct Light { bool active, overridden; Col colour; int refreshes; };
static void SetColour(Light& l, const Col& c)
{
    Col black = { 0, 0, 0, 255 };
    if (l.overridden && !Eq(l.colour, c)) {
        if (Eq(l.colour, black)) l.active = true;
        l.colour = c;
        if (Eq(l.colour, black)) l.active = false;
        l.refreshes++;
    }
}
int main()
{
    Col black = { 0, 0, 0, 255 }, red = { 0, 0, 255, 255 };
    Light l = { false, true, black, 0 };
    SetColour(l, red);
    if (!l.active || l.colour.R != 255 || l.refreshes != 1) { std::printf("00640bb0_TEST FAIL\n"); return 1; }
    SetColour(l, red);
    if (l.refreshes != 1) { std::printf("00640bb0_TEST FAIL\n"); return 1; }
    SetColour(l, black);
    if (l.active || l.refreshes != 2) { std::printf("00640bb0_TEST FAIL\n"); return 1; }
    Light n = { false, false, black, 0 };
    SetColour(n, red);
    if (n.refreshes != 0 || n.colour.R != 0) { std::printf("00640bb0_TEST FAIL\n"); return 1; }
    std::printf("00640bb0_TEST PASS\n");
    return 0;
}
