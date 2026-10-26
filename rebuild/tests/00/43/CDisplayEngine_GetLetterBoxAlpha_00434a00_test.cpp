#include <stdio.h>
#include <stddef.h>
class CFadeInFadeOutBase { public: bool Active; float FadeInTime; float FadeOutTime; bool Closing; bool Opening; float OpenTimer; float CloseTimer; unsigned long ToColour; };
static const void* g_this;
struct CFadeInFadeOutBase_Methods : CFadeInFadeOutBase {
    float GetFadeInOutAlpha() const { g_this = this; return Active ? OpenTimer : 0.25f; }
};
struct CLetterBoxModeInfo : CFadeInFadeOutBase_Methods { float Ratio; };
class CDisplayEngine { public: unsigned char _pad[0x70]; unsigned char LetterBox[0x20]; bool CameraInputsOn; };
struct CDisplayEngine_Methods : CDisplayEngine { float GetLetterBoxAlpha() const; };

float CDisplayEngine_Methods::GetLetterBoxAlpha() const
{
    float alpha = ((const CLetterBoxModeInfo*)LetterBox)->GetFadeInOutAlpha();
    return alpha;
}

int main()
{
    static CDisplayEngine_Methods e;
    CLetterBoxModeInfo* lb = (CLetterBoxModeInfo*)e.LetterBox;
    lb->Active = true; lb->OpenTimer = 0.75f;
    float a = e.GetLetterBoxAlpha();
    bool ok = a == 0.75f && g_this == (const void*)((char*)&e + 0x70) && offsetof(CDisplayEngine, LetterBox) == 0x70;
    lb->Active = false;
    ok = ok && e.GetLetterBoxAlpha() == 0.25f;
    printf(ok ? "00434a00_TEST PASS\n" : "00434a00_TEST FAIL\n");
    return ok ? 0 : 1;
}