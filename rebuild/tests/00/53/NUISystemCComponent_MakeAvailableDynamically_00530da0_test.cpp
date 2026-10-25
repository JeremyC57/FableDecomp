// Behaviour test (0x00530da0): NUISystem::CComponent::MakeAvailableDynamically scales R, G, B of every UI state by 4/3, then fades to the current state's colour keeping the component's own alpha
#include <cstdio>
#include <cmath>
struct CRGBColour { unsigned char B, G, R, A; void Set(int r, int g, int b, int a) { R = (unsigned char)r; G = (unsigned char)g; B = (unsigned char)b; A = (unsigned char)a; } };
struct CUIState { CRGBColour Colour; };
struct CComponent {
    CUIState States[3]; int NumStates; int Current; CRGBColour Colour;
    CRGBColour Target; float FadeTime; bool Flag; int ChangeCalls;
    CRGBColour GetColour() const { return Colour; }
    CUIState* GetCurrentState() { return &States[Current]; }
    void ChangeColour(const CRGBColour& c, float time, bool flag) { Target = c; FadeTime = time; Flag = flag; ++ChangeCalls; }
    void MakeAvailable()
    {
        for (int i = 0; i != NumStates; ++i) {
            CRGBColour& colour = States[i].Colour;
            colour.R = (unsigned char)(colour.R * 1.33333337f);
            colour.G = (unsigned char)(colour.G * 1.33333337f);
            colour.B = (unsigned char)(colour.B * 1.33333337f);
        }
    }
    void MakeAvailableDynamically()
    {
        MakeAvailable();
        CRGBColour colour = GetCurrentState()->Colour;
        colour.A = GetColour().A;
        ChangeColour(colour, 0.0f, false);
    }
};
int main()
{
    CComponent c; c.NumStates = 2; c.Current = 1; c.ChangeCalls = 0; c.FadeTime = 5.0f; c.Flag = true;
    c.States[0].Colour.Set(90, 30, 3, 10);
    c.States[1].Colour.Set(150, 60, 0, 20);
    c.Colour.Set(1, 2, 3, 77);
    c.MakeAvailableDynamically();
    if (c.States[0].Colour.R != 120 || c.States[0].Colour.G != 40 || c.States[0].Colour.B != 4 || c.States[0].Colour.A != 10) { std::printf("00530da0_TEST FAIL\n"); return 1; }
    if (c.ChangeCalls != 1 || c.Target.R != 200 || c.Target.G != 80 || c.Target.B != 0 || c.Target.A != 77 || std::fabs(c.FadeTime) > 1e-6f || c.Flag) { std::printf("00530da0_TEST FAIL\n"); return 1; }
    std::printf("00530da0_TEST PASS\n");
    return 0;
}