#include <cstdio>
#include "engine/CDisplayEngine.h"
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CFadeInFadeOutBase_Open {                         // CFadeInFadeOutBase with ToColour typed
    bool       Active;                                   // +0x00
    float      FadeInTime;                               // +0x04
    float      FadeOutTime;                              // +0x08
    bool       Closing;                                  // +0x0c
    bool       Opening;                                  // +0x0d
    float      OpenTimer;                                // +0x10
    float      CloseTimer;                               // +0x14
    CRGBColour ToColour;                                 // +0x18
    void Open(float fade_in_time, float fade_out_time, CRGBColour to_colour)   // 0x00434840
    {
        FadeInTime = fade_in_time;
        FadeOutTime = fade_out_time;
        Active = true;
        Closing = false;
        Opening = true;
        OpenTimer = 0.0f;
        CloseTimer = fade_out_time;
        ToColour = to_colour;
    }
};
struct CLetterBoxModeInfo : CFadeInFadeOutBase_Open {
    float Ratio;                                         // +0x1c
};
struct CDisplayEngine_Methods : CDisplayEngine {
    CLetterBoxModeInfo& GetLetterBox() { return *reinterpret_cast<CLetterBoxModeInfo*>(LetterBox); }
    void StartLetterBoxMode(float fade_in_time, float fade_out_time, float ratio);
};
void CDisplayEngine_Methods::StartLetterBoxMode(float fade_in_time, float fade_out_time, float ratio)
{
    GetLetterBox().Open(fade_in_time, fade_out_time, CRGBColour(0, 0, 0, 255));
    GetLetterBox().Ratio = ratio;
}

int main()
{
    static unsigned char raw[sizeof(CDisplayEngine)];
    CDisplayEngine_Methods* e = reinterpret_cast<CDisplayEngine_Methods*>(raw);
    for (int i = 0; i < (int)sizeof(raw); ++i) raw[i] = 0x5a;
    e->StartLetterBoxMode(1.5f, 2.5f, 0.75f);
    CLetterBoxModeInfo& lb = e->GetLetterBox();
    bool ok = lb.Active == true && lb.FadeInTime == 1.5f && lb.FadeOutTime == 2.5f && lb.Closing == false && lb.Opening == true
           && lb.OpenTimer == 0.0f && lb.CloseTimer == 2.5f && lb.ToColour.R == 0 && lb.ToColour.G == 0 && lb.ToColour.B == 0
           && lb.ToColour.A == 255 && lb.Ratio == 0.75f;
    ok = ok && raw[0x70] == 1 && *(float*)(raw + 0x8c) == 0.75f && raw[0x8b] == 0xff && raw[0x90] == 0x5a && raw[0x6f] == 0x5a;
    std::printf(ok ? "00434ba0_TEST PASS\n" : "00434ba0_TEST FAIL\n");
    return ok ? 0 : 1;
}