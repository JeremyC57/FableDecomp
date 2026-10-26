#include <cstdio>

static int g_render = 0;
class CThing { public: void AddToRenderList() { ++g_render; } };
struct CTCGraphicAppearance {
    CThing* ParentThing;
    unsigned char UpdatePrimitiveReasons;
    unsigned char SelfIlluminating : 1;
    unsigned char ForceResendEveryRenderFrame : 1;
    unsigned char _rest : 6;
    void SetSelfIlluminating(bool selfIlluminating)
    {
        SelfIlluminating = selfIlluminating;
        ParentThing->AddToRenderList();
        UpdatePrimitiveReasons |= 4;
    }
};

int main()
{
    CThing t; CTCGraphicAppearance a; a.ParentThing = &t; a.UpdatePrimitiveReasons = 1;
    a.SelfIlluminating = 0; a.ForceResendEveryRenderFrame = 1; a._rest = 0x2a;
    a.SetSelfIlluminating(true);
    if (!a.SelfIlluminating || !a.ForceResendEveryRenderFrame || a._rest != 0x2a || a.UpdatePrimitiveReasons != 5 || g_render != 1)
        { std::printf("004bfa20_TEST FAIL\n"); return 1; }
    a.SetSelfIlluminating(false);
    if (a.SelfIlluminating || !a.ForceResendEveryRenderFrame || g_render != 2) { std::printf("004bfa20_TEST FAIL\n"); return 1; }
    std::printf("004bfa20_TEST PASS\n");
    return 0;
}