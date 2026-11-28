#include <cstdio>

static unsigned long g_reasons = 0; static float g_max = 0.0f;
#include "engine/GFFloatRandom.h"
static unsigned long seed=17;
float FABLE_FASTCALL GFFloatRandom(float max,unsigned long* p) {g_max=max;if(p!=&seed)return -1;return .25f*max;}
struct CRandomGenerator { unsigned long* m_seed; };
class CDisplayEngine { public: CRandomGenerator m_random; CRandomGenerator* GetRandomGenerator() { return &m_random; } };
static CDisplayEngine g_engine;
struct CTCGraphicAppearance { void SetAsChanged(unsigned long reasons) { g_reasons |= reasons; } };
class CThing { public: CTCGraphicAppearance* PTCGraphicAppearance; };
class CTCBase { public:
    virtual ~CTCBase() { }
    CThing* ParentThing;
    CDisplayEngine* GetDisplayEngine() { return &g_engine; }
};
struct CTCGraphicAppearanceSprite : CTCBase {
    float RotationAmountPerFrame, RotationAngle;
    void SetRotationAngle(float amount)
    {
        ParentThing->PTCGraphicAppearance->SetAsChanged(4);
        RotationAmountPerFrame = amount;
        unsigned long* random = GetDisplayEngine()->GetRandomGenerator()->m_seed;
        RotationAngle = GFFloatRandom(1.0f,random);
    }
};

int main()
{
    CTCGraphicAppearance app; CThing owner; owner.PTCGraphicAppearance = &app;
    CTCGraphicAppearanceSprite s; s.ParentThing = &owner; s.RotationAmountPerFrame = 0.0f; s.RotationAngle = 0.0f;
    g_engine.m_random.m_seed = &seed;
    s.SetRotationAngle(0.125f);
    if (g_reasons != 4 || s.RotationAmountPerFrame != 0.125f || s.RotationAngle != 0.25f || g_max != 1.0f)
        { std::printf("0077c490_TEST FAIL\n"); return 1; }
    std::printf("0077c490_TEST PASS\n");
    return 0;
}