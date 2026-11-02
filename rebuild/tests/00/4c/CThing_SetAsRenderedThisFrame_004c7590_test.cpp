#include <stdio.h>
static int g_calls; static unsigned long g_flags; static const void* g_this;
class CTCGraphicAppearance { public: int x; void SetAsChanged(unsigned long flags) { ++g_calls; g_flags = flags; g_this = this; } };
class CThing {
public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;
    void SetGraphicAppearanceChanged(unsigned long flags);
};

void CThing::SetGraphicAppearanceChanged(unsigned long flags)
{
    if (PTCGraphicAppearance)
        PTCGraphicAppearance->SetAsChanged(flags);
}

int main()
{
    static CThing t; CTCGraphicAppearance a;
    t.PTCGraphicAppearance = 0; t.SetGraphicAppearanceChanged(3);
    bool ok = g_calls == 0;
    t.PTCGraphicAppearance = &a; t.SetGraphicAppearanceChanged(0x40);
    ok = ok && g_calls == 1 && g_flags == 0x40 && g_this == &a;
    printf(ok ? "004c7590_TEST PASS\n" : "004c7590_TEST FAIL\n");
    return ok ? 0 : 1;
}