#include <cstdio>
class CRGBColour { public: unsigned long Colour; };
struct CComponent {
    CRGBColour Colour, TargetColour, InitialColour;
    void SetColour(const CRGBColour& colour) { CRGBColour v = colour; TargetColour = v; InitialColour = v; Colour = v; }
};
int main()
{
    CComponent c;
    c.Colour.Colour = 1; c.TargetColour.Colour = 2; c.InitialColour.Colour = 3;
    CRGBColour w; w.Colour = 0xff102030ul;
    c.SetColour(w);
    if (c.Colour.Colour != 0xff102030ul || c.TargetColour.Colour != 0xff102030ul || c.InitialColour.Colour != 0xff102030ul)
        { std::printf("0052e7a0_TEST FAIL\n"); return 1; }
    c.Colour.Colour = 7;
    c.SetColour(c.Colour);   /* aliasing: the value read once */
    if (c.TargetColour.Colour != 7 || c.InitialColour.Colour != 7 || c.Colour.Colour != 7) { std::printf("0052e7a0_TEST FAIL\n"); return 1; }
    std::printf("0052e7a0_TEST PASS\n");
    return 0;
}