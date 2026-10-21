// test for 00434840 CFadeInFadeOutBase::Open
#include <stdio.h>
#include <string.h>
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CFadeInFadeOutBase {
    bool       Active;                                   // +0x00
    float      FadeInTime;                               // +0x04
    float      FadeOutTime;                              // +0x08
    bool       Closing;                                  // +0x0c
    bool       Opening;                                  // +0x0d
    float      OpenTimer;                                // +0x10
    float      CloseTimer;                               // +0x14
    CRGBColour ToColour;                                 // +0x18
    void Open(float fade_in_time, float fade_out_time, CRGBColour to_colour);
};
void CFadeInFadeOutBase::Open(float fade_in_time, float fade_out_time, CRGBColour to_colour)
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

int main()
{
    CFadeInFadeOutBase f; memset(&f, 0x5a, sizeof f);
    f.Open(1.5f, 2.5f, CRGBColour(10, 20, 30, 40));
    int ok = sizeof f == 0x1c && f.Active && f.Opening && !f.Closing && f.FadeInTime == 1.5f && f.FadeOutTime == 2.5f
          && f.OpenTimer == 0.0f && f.CloseTimer == 2.5f && f.ToColour.R == 10 && f.ToColour.G == 20 && f.ToColour.B == 30 && f.ToColour.A == 40;
    printf(ok ? "00434840_TEST PASS\n" : "00434840_TEST FAIL\n");
    return 0;
}