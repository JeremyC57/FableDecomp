// behaviour test for 007786f0 CTCTimeAppearanceFade::FrameUpdate: alpha 255 between fade-in end and fade-out start,
// a ramp up across the fade-in window, a ramp down across the fade-out window, 0 outside; nothing without an appearance
#include <cstdio>
class CThing;
static int g_alpha = -1, g_destroyed = 0;
namespace NGraphicAppearance {
struct CTCGraphicAppearance {
    void SetAlpha(unsigned char alpha) { g_alpha = alpha; }
};
}
bool GFGetValPositionInLimitsLooping(float value, float start, float end, float minLimit, float maxLimit, float* position)
{   // test stub: the non-wrapping window of the retail helper
    (void)minLimit; (void)maxLimit;
    if (value < start || value > end)
        return false;
    *position = (value - start) / (end - start);
    return true;
}
struct CTimeAppearanceFadeDef {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long  m_count;
    float FadeInStart, FadeInEnd, FadeOutStart, FadeOutEnd;
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
struct CFadeDefPointer {
    CTimeAppearanceFadeDef* Object;
    CFadeDefPointer() : Object(0) {}
    ~CFadeDefPointer() { if (Object) Object->Release(); }
    CTimeAppearanceFadeDef* operator->() const { return Object; }
};
static CTimeAppearanceFadeDef g_def;
struct CDefClassBase {
    bool GetSubDef(CFadeDefPointer* out) { ++g_def.m_count; out->Object = &g_def; return true; }
};
class CThing {
public:
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;
    CDefClassBase*  DefClass;
};
struct CGameTimeManager { float Time; };
struct CWorld { CGameTimeManager* PGameTimeManager; };
static CGameTimeManager g_time; static CWorld g_world;
struct CTCBase {
    virtual void v00() {}
    CThing* ParentThing;
    CWorld* GetWorld() { return &g_world; }
};
struct CTCTimeAppearanceFade : CTCBase {
    virtual void FrameUpdate();
};
void CTCTimeAppearanceFade::FrameUpdate()
{
    NGraphicAppearance::CTCGraphicAppearance* appearance = ParentThing->PTCGraphicAppearance;
    if (!appearance)
        return;
    CDefClassBase* defClass = ParentThing->DefClass;
    CFadeDefPointer def;
    if (defClass->GetSubDef(&def)) {
        float time = GetWorld()->PGameTimeManager->Time;
        float position = 0.0f;
        if (GFGetValPositionInLimitsLooping(time, def->FadeInEnd, def->FadeOutStart, 0.0f, 1.0f, &position))
            appearance->SetAlpha(0xff);
        else if (GFGetValPositionInLimitsLooping(time, def->FadeInStart, def->FadeInEnd, 0.0f, 1.0f, &position))
            appearance->SetAlpha((unsigned char)(long)(position * 255.0f));
        else if (GFGetValPositionInLimitsLooping(time, def->FadeOutStart, def->FadeOutEnd, 0.0f, 1.0f, &position))
            appearance->SetAlpha((unsigned char)(long)((1.0f - position) * 255.0f));
        else
            appearance->SetAlpha(0);
    }
}
int main()
{
    g_world.PGameTimeManager = &g_time; g_def.m_count = 0;
    g_def.FadeInStart = 0.2f; g_def.FadeInEnd = 0.3f; g_def.FadeOutStart = 0.7f; g_def.FadeOutEnd = 0.8f;
    NGraphicAppearance::CTCGraphicAppearance appearance; CDefClassBase defClass;
    CThing thing; thing.PTCGraphicAppearance = &appearance; thing.DefClass = &defClass;
    CTCTimeAppearanceFade fade; fade.ParentThing = &thing;
    g_time.Time = 0.5f; fade.FrameUpdate(); bool ok = g_alpha == 255;
    g_time.Time = 0.25f; fade.FrameUpdate(); ok = ok && g_alpha >= 126 && g_alpha <= 128;
    g_time.Time = 0.75f; fade.FrameUpdate(); ok = ok && g_alpha >= 126 && g_alpha <= 128;
    g_time.Time = 0.9f; fade.FrameUpdate(); ok = ok && g_alpha == 0 && g_destroyed == 4;
    thing.PTCGraphicAppearance = 0; g_alpha = -1; fade.FrameUpdate(); ok = ok && g_alpha == -1 && g_destroyed == 4;
    if (!ok) { std::printf("007786f0_TEST FAIL\n"); return 1; }
    std::printf("007786f0_TEST PASS\n");
    return 0;
}