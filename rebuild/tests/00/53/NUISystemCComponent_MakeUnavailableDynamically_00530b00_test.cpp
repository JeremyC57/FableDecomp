// Behaviour test (0x00530b00): NUISystem::CComponent::MakeUnavailableDynamically scales R, G, B of every UI state by 0.75, then the fade's target, initial and current colours too (alpha untouched)
#include <cstdio>
class CRGBColour { public: unsigned char B, G, R, A; void Set(int r, int g, int b, int a) { R = (unsigned char)r; G = (unsigned char)g; B = (unsigned char)b; A = (unsigned char)a; } };
struct CUIState { CRGBColour Colour; };
static void Darken(CRGBColour& c) { c.R = (unsigned char)(c.R * 0.75f); c.G = (unsigned char)(c.G * 0.75f); c.B = (unsigned char)(c.B * 0.75f); }
struct CComponent {
    CUIState States[2]; int NumStates; CRGBColour Colour; CRGBColour TargetColour; CRGBColour InitialColour;
    void MakeUnavailable() { for (int i = 0; i != NumStates; ++i) Darken(States[i].Colour); }
    void MakeUnavailableDynamically()
    {
        MakeUnavailable();
        TargetColour.R = (unsigned char)(TargetColour.R * 0.75f);
        TargetColour.G = (unsigned char)(TargetColour.G * 0.75f);
        TargetColour.B = (unsigned char)(TargetColour.B * 0.75f);
        InitialColour.R = (unsigned char)(InitialColour.R * 0.75f);
        InitialColour.G = (unsigned char)(InitialColour.G * 0.75f);
        InitialColour.B = (unsigned char)(InitialColour.B * 0.75f);
        Colour.R = (unsigned char)(Colour.R * 0.75f);
        Colour.G = (unsigned char)(Colour.G * 0.75f);
        Colour.B = (unsigned char)(Colour.B * 0.75f);
    }
};
int main()
{
    CComponent c; c.NumStates = 2;
    c.States[0].Colour.Set(200, 100, 4, 255); c.States[1].Colour.Set(255, 1, 0, 128);
    c.TargetColour.Set(40, 80, 120, 9); c.InitialColour.Set(8, 16, 33, 10); c.Colour.Set(255, 255, 255, 11);
    c.MakeUnavailableDynamically();
    if (c.States[0].Colour.R != 150 || c.States[0].Colour.G != 75 || c.States[0].Colour.B != 3 || c.States[0].Colour.A != 255) { std::printf("00530b00_TEST FAIL\n"); return 1; }
    if (c.States[1].Colour.R != 191 || c.States[1].Colour.G != 0 || c.TargetColour.R != 30 || c.TargetColour.B != 90 || c.TargetColour.A != 9) { std::printf("00530b00_TEST FAIL\n"); return 1; }
    if (c.InitialColour.R != 6 || c.InitialColour.G != 12 || c.InitialColour.B != 24 || c.Colour.R != 191 || c.Colour.A != 11) { std::printf("00530b00_TEST FAIL\n"); return 1; }
    std::printf("00530b00_TEST PASS\n");
    return 0;
}