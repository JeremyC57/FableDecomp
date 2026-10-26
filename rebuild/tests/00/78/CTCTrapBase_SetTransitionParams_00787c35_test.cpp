#include <cstdio>

// Model: CTCTrapBase::SetTransitionParams forwards both fade times and opaque black to the opener fader.
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& o) { B = o.B; G = o.G; R = o.R; A = o.A; }
};

class CFadeInFadeOutBase { public:
    bool       Active;
    float      FadeInTime;
    float      FadeOutTime;
    bool       Closing;
    bool       Opening;
    float      OpenTimer;
    float      CloseTimer;
    CRGBColour ToColour;
    CFadeInFadeOutBase() : ToColour(1, 2, 3, 4) { }
    void Open(float fade_in_time, float fade_out_time, CRGBColour to_colour)
    {
        Active = true;
        FadeInTime = fade_in_time;
        FadeOutTime = fade_out_time;
        Opening = true;
        Closing = false;
        OpenTimer = 0.0f;
        CloseTimer = fade_out_time;
        ToColour = to_colour;
    }
};

class CTCTrapBase {
public:
    CFadeInFadeOutBase TrapOpener;
    void SetTransitionParams(float fadeInTime, float fadeOutTime)
    {
        TrapOpener.Open(fadeInTime, fadeOutTime, CRGBColour(0, 0, 0, 255));
    }
};

int main()
{
    CTCTrapBase trap;
    trap.TrapOpener.Active = false;
    trap.TrapOpener.Closing = true;
    trap.SetTransitionParams(0.5f, 2.25f);
    const CFadeInFadeOutBase& f = trap.TrapOpener;
    if (!f.Active || f.FadeInTime != 0.5f || f.FadeOutTime != 2.25f || f.Closing || !f.Opening || f.OpenTimer != 0.0f
        || f.CloseTimer != 2.25f || f.ToColour.R != 0 || f.ToColour.G != 0 || f.ToColour.B != 0 || f.ToColour.A != 255) {
        std::printf("00787c35_TEST FAIL\n");
        return 1;
    }
    std::printf("00787c35_TEST PASS\n");
    return 0;
}