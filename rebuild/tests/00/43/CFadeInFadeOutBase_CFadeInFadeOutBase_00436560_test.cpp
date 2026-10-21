// test for 00436560 CFadeInFadeOutBase ctor
#include <stdio.h>
#include <string.h>
#include <new>
#include <stddef.h>
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
    CFadeInFadeOutBase();
};
CFadeInFadeOutBase::CFadeInFadeOutBase()
{
    Active = false;
    CloseTimer = 0.0f;
    OpenTimer = 0.0f;
    Opening = false;
    Closing = false;
    ToColour = CRGBColour(0, 0, 0, 255);
}

int main()
{
    unsigned char buf[sizeof(CFadeInFadeOutBase)];
    memset(buf, 0x5a, sizeof buf);
    CFadeInFadeOutBase* f = new (buf) CFadeInFadeOutBase;
    int ok = sizeof(CFadeInFadeOutBase) == 0x1c && !f->Active && !f->Closing && !f->Opening && f->OpenTimer == 0.0f && f->CloseTimer == 0.0f
          && buf[0x18] == 0 && buf[0x19] == 0 && buf[0x1a] == 0 && buf[0x1b] == 0xff && buf[4] == 0x5a && buf[8] == 0x5a;
    printf(ok ? "00436560_TEST PASS\n" : "00436560_TEST FAIL\n");
    return 0;
}