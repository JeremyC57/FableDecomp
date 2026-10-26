// Behaviour test for CDisplacementProcess::Process (0x00880D50): current = current * (1 - w) + entry colour * w.
#include <cstdio>
class CRGBFloatColour { public:
    float R, G, B, A;
    CRGBFloatColour() { }
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
    CRGBFloatColour operator*(float scale) const { return CRGBFloatColour(R * scale, G * scale, B * scale, A * scale); }
    CRGBFloatColour operator+(const CRGBFloatColour& o) const { return CRGBFloatColour(R + o.R, G + o.G, B + o.B, A + o.A); }
};
struct CStackEntry { long curdurationcount, durationcount; bool fadingout; };
struct CDisplacementMonochromeEffectColourEntry : CStackEntry { CRGBFloatColour Colour; };
struct CScreenEffectPriorityStack { CRGBFloatColour CurrentDisplacementParam; };
struct CDisplacementProcess {
    CScreenEffectPriorityStack* Owner;
    void Process(float weight, const CStackEntry* entry);
};
void CDisplacementProcess::Process(float weight, const CStackEntry* entry)
{
    const CDisplacementMonochromeEffectColourEntry* colourEntry = static_cast<const CDisplacementMonochromeEffectColourEntry*>(entry);
    float remaining = 1.0f - weight;
    Owner->CurrentDisplacementParam = Owner->CurrentDisplacementParam * remaining + colourEntry->Colour * weight;
}
static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }
int main()
{
    CScreenEffectPriorityStack stack; stack.CurrentDisplacementParam = CRGBFloatColour(1.0f, 0.0f, 0.5f, 1.0f);
    CDisplacementMonochromeEffectColourEntry e; e.Colour = CRGBFloatColour(0.0f, 1.0f, 0.5f, 0.0f);
    CDisplacementProcess p; p.Owner = &stack;
    p.Process(0.25f, &e);
    CRGBFloatColour c = stack.CurrentDisplacementParam;
    int bad = !(Near(c.R, 0.75f) && Near(c.G, 0.25f) && Near(c.B, 0.5f) && Near(c.A, 0.75f));
    p.Process(1.0f, &e);
    c = stack.CurrentDisplacementParam;
    if (!(Near(c.R, 0.0f) && Near(c.G, 1.0f) && Near(c.A, 0.0f))) bad = 2;
    if (bad) { std::printf("00880d50_TEST FAIL\n"); return 1; }
    std::printf("00880d50_TEST PASS\n");
    return 0;
}