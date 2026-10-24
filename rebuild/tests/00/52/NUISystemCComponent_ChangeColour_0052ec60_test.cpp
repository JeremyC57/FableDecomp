// Behaviour test for 0x0052EC60 NUISystem::CComponent::ChangeColour: a timed change records target/initial and restarts the timer, an instant change sets all three
#include <cstdio>
struct CRGBColour { unsigned char B, G, R, A; };
struct CComponent {
    unsigned char _pad[0x84];
    CRGBColour Colour;
    CRGBColour TargetColour;
    CRGBColour InitialColour;
    unsigned char _pad90[0xa8 - 0x90];
    float ColourTimeElapsed;
    float ColourTime;
    void ChangeColour(const CRGBColour& colour, float time, bool unused);
};
void CComponent::ChangeColour(const CRGBColour& colour, float time, bool unused)
{
    ColourTime = time;
    if (time > 0.0f) {
        TargetColour = colour;
        InitialColour = Colour;
        ColourTimeElapsed = 0.0f;
    } else {
        TargetColour = colour;
        InitialColour = colour;
        Colour = colour;
    }
}
static CRGBColour Make(int b, int g, int r, int a)
{
    CRGBColour c; c.B = (unsigned char)b; c.G = (unsigned char)g; c.R = (unsigned char)r; c.A = (unsigned char)a; return c;
}
int main()
{
    CComponent c;
    c.Colour = Make(1, 2, 3, 4);
    c.ColourTimeElapsed = 5.0f;
    c.ChangeColour(Make(10, 20, 30, 40), 1.5f, false);
    if (c.ColourTime != 1.5f || c.TargetColour.R != 30 || c.InitialColour.R != 3 || c.Colour.A != 4 || c.ColourTimeElapsed != 0.0f)
        { std::printf("0052ec60_TEST FAIL\n"); return 1; }
    c.ColourTimeElapsed = 6.0f;
    c.ChangeColour(Make(50, 60, 70, 80), -1.0f, false);
    if (c.Colour.B != 50 || c.Colour.A != 80 || c.InitialColour.G != 60 || c.TargetColour.R != 70 || c.ColourTimeElapsed != 6.0f)
        { std::printf("0052ec60_TEST FAIL\n"); return 1; }
    std::printf("0052ec60_TEST PASS\n");
    return 0;
}