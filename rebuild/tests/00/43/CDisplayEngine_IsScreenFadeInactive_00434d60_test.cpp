#include <stdio.h>
struct CFadeInFadeOutBase { bool Active; float FadeInTime; float FadeOutTime; bool Closing; bool Opening; float OpenTimer; float CloseTimer; unsigned long ToColour; };
struct CDisplayEngine { unsigned char _pad[0xbc]; unsigned char ScreenFadeOutInfo[0x1c]; };
struct CDisplayEngine_Methods : CDisplayEngine { bool IsScreenFadeInactive() const; };

bool CDisplayEngine_Methods::IsScreenFadeInactive() const
{
    return ((const CFadeInFadeOutBase*)ScreenFadeOutInfo)->Active ? false : true;
}

int main()
{
    static CDisplayEngine_Methods e;
    CFadeInFadeOutBase* f = (CFadeInFadeOutBase*)e.ScreenFadeOutInfo;
    f->Active = false; bool a = e.IsScreenFadeInactive();
    f->Active = true;  bool b = e.IsScreenFadeInactive();
    bool ok = a && !b && (char*)f - (char*)&e == 0xbc;
    printf(ok ? "00434d60_TEST PASS\n" : "00434d60_TEST FAIL\n");
    return ok ? 0 : 1;
}