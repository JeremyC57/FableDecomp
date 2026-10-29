// Behaviour test for the balverine fade FrameUpdate (0x008CC7A0): alpha from the percentage complete (retail: (1 - 2p) x 255 past half way, non-positive, so the byte wraps; constant 1.0 at 0x0122DED8 read from .rdata), else 255;
// the base FrameUpdate always runs.
#include <stdio.h>
static unsigned char g_alpha = 0; static int g_baseUpdates = 0;
class CTCGraphicAppearance { public: void SetAlpha(unsigned char a) { g_alpha = a; } };
class CThingCreatureBase { public:
    CTCGraphicAppearance App;
    CTCGraphicAppearance* GetTCGraphicAppearance() { return &App; }
};
class CCombatActionBase { public:
    CThingCreatureBase* PCreature; float Percent;
    float GetPercentageComplete() const { return Percent; }
    void FrameUpdate() { g_baseUpdates++; }
};
class CFadeAction : public CCombatActionBase { public:
    void FrameUpdate()
    {
        if (GetPercentageComplete() > 0.5f) {
            float alpha = 1.0f - GetPercentageComplete() * 2.0f;
            PCreature->GetTCGraphicAppearance()->SetAlpha((unsigned char)(long)(alpha * 255.0f));
        } else {
            PCreature->GetTCGraphicAppearance()->SetAlpha(255);
        }
        CCombatActionBase::FrameUpdate();
    }
};
static unsigned char Run(float p)
{
    CThingCreatureBase c; CFadeAction a; a.PCreature = &c; a.Percent = p;
    g_alpha = 1; a.FrameUpdate(); return g_alpha;
}
int main()
{
    int ok = Run(0.0f) == 255 && Run(0.5f) == 255 && Run(0.75f) == 129 && Run(1.0f) == 1 && Run(0.25f) == 255;
    if (!ok || g_baseUpdates != 5) { printf("008cc7a0_TEST FAIL\n"); return 1; }
    printf("008cc7a0_TEST PASS\n");
    return 0;
}