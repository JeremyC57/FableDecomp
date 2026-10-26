// behaviour test for 0x004364C0
#include <stdio.h>
#include <stddef.h>
class CFadeInFadeOutBase { public: bool Active; float FadeInTime; float FadeOutTime; bool Closing; bool Opening; float OpenTimer; float CloseTimer; unsigned long ToColour; };
static const void* g_this;
struct CFadeInFadeOutBase_Methods : CFadeInFadeOutBase {
    float GetFadeInOutAlpha() const { g_this = this; return Active ? OpenTimer : 0.0f; }
};
class CDisplayEngine { public: unsigned char _pad[0xbc]; unsigned char ScreenFadeOutInfo[0x1c]; bool ScreenFadeOutLocked; };
struct CDisplayEngine_Methods : CDisplayEngine {
    unsigned char GetScreenFadeOutAlpha() const;
};

unsigned char CDisplayEngine_Methods::GetScreenFadeOutAlpha() const
{
    return (unsigned char)(((const CFadeInFadeOutBase_Methods*)ScreenFadeOutInfo)->GetFadeInOutAlpha() * 255.0f);
}
int main()
{
    static CDisplayEngine_Methods e;
    CFadeInFadeOutBase* f = (CFadeInFadeOutBase*)e.ScreenFadeOutInfo;
    f->Active = true; f->OpenTimer = 1.0f;
    bool ok = e.GetScreenFadeOutAlpha() == 255 && g_this == (const void*)((char*)&e + 0xbc);
    f->OpenTimer = 0.5f; ok = ok && e.GetScreenFadeOutAlpha() == 127;
    f->Active = false; ok = ok && e.GetScreenFadeOutAlpha() == 0;
    printf(ok ? "004364c0_TEST PASS\n" : "004364c0_TEST FAIL\n");
    return ok ? 0 : 1;
}