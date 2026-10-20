// Behaviour test for CTCFadeOutAndIn::FrameUpdate (retail 0x006C3F40): a copy of the model with stub callees.
#include <cstdio>

static int g_removedType = 0;
static int g_removeDeadCalls = 0;
static unsigned char g_lastAlpha = 0;
static int g_alphaCalls = 0;
static int g_drawableCalls = 0;
static bool g_lastDrawable = false;

struct CTCGraphicAppearance {
    unsigned char alpha;
    void SetAlpha(unsigned char a) { alpha = a; g_lastAlpha = a; ++g_alphaCalls; }
};

class CThing {
public:
    virtual void SetAsDrawable(bool drawable) { Drawable = drawable; g_lastDrawable = drawable; ++g_drawableCalls; }
    CTCGraphicAppearance* PGraphicAppearance;
    bool Drawable;
    void RemoveTC(long interfaceType) { g_removedType = (int)interfaceType; }
};

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};

template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end;
};

float g_epsilon_0129ba3c = 1e-4f;

class CTCFadeOutAndIn {
public:
    CThing* ParentThing;
    bool  FadingOut;
    bool  FadingIn;
    float Alpha;
    long  FramesLeftForFade;
    CRetailVector<CBaseIntelligentPointer> FadingThings;
    void RemoveDeadFadingThings() { ++g_removeDeadCalls; }
    void FrameUpdate();
};

void CTCFadeOutAndIn::FrameUpdate()
{
    if (ParentThing->PGraphicAppearance == 0) {
        FadingOut = false;
        FadingIn = false;
        ParentThing->RemoveTC(0xf9);
        return;
    }
    RemoveDeadFadingThings();
    if (!FadingOut && !FadingIn)
        return;
    long frames = FramesLeftForFade;
    if (frames <= 0) {
        if (FadingIn) {
            FadingIn = false;
            ParentThing->RemoveTC(0xf9);
            return;
        }
        FadingOut = false;
        return;
    }
    float amount;
    float direction;
    if (FadingIn) {
        amount = 1.0f - Alpha;
        direction = 1.0f;
    } else {
        amount = Alpha;
        direction = -1.0f;
    }
    FramesLeftForFade = frames - 1;
    Alpha += direction * (amount / frames);
    for (CBaseIntelligentPointer* it = FadingThings._M_start; it != FadingThings._M_finish; ++it) {
        CTCGraphicAppearance* appearance = it->GetPItem()->PGraphicAppearance;
        if (appearance) {
            appearance->SetAlpha((unsigned char)(Alpha * 255.0f));
            it->GetPItem()->SetAsDrawable(Alpha > g_epsilon_0129ba3c ? true : false);
        }
    }
}

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    int fail = 0;
    CTCGraphicAppearance ga1 = { 0 }, ga2 = { 0 };
    CThing parent; parent.PGraphicAppearance = &ga1; parent.Drawable = false;
    CThing other; other.PGraphicAppearance = &ga2; other.Drawable = false;
    CThing bare; bare.PGraphicAppearance = 0; bare.Drawable = false;
    CBaseIntelligentPointer items[3];
    items[0].m_pItem = &parent; items[1].m_pItem = &bare; items[2].m_pItem = &other;

    CTCFadeOutAndIn f;
    f.ParentThing = &parent; f.FadingThings._M_start = items; f.FadingThings._M_finish = items + 3; f.FadingThings._M_end = items + 3;

    // 1. fading out over 4 frames from alpha 1: 1 - 1/4 = 0.75
    f.FadingOut = true; f.FadingIn = false; f.Alpha = 1.0f; f.FramesLeftForFade = 4;
    f.FrameUpdate();
    if (!Near(f.Alpha, 0.75f) || f.FramesLeftForFade != 3 || g_removeDeadCalls != 1) fail = 1;
    if (ga1.alpha != 191 || ga2.alpha != 191 || g_alphaCalls != 2 || !parent.Drawable || !other.Drawable || bare.Drawable) fail = 2;

    // 2. fading in over 2 frames from 0.5: 0.5 + 0.5/2 = 0.75
    f.FadingOut = false; f.FadingIn = true; f.Alpha = 0.5f; f.FramesLeftForFade = 2;
    f.FrameUpdate();
    if (!Near(f.Alpha, 0.75f) || f.FramesLeftForFade != 1) fail = 3;

    // 3. fade out to zero: alpha 0.25 over 1 frame -> 0, not drawable
    f.FadingOut = true; f.FadingIn = false; f.Alpha = 0.25f; f.FramesLeftForFade = 1;
    f.FrameUpdate();
    if (!Near(f.Alpha, 0.0f) || f.FramesLeftForFade != 0 || parent.Drawable || other.Drawable || ga1.alpha != 0) fail = 4;

    // 4. no frames left while fading out: just stop
    g_removedType = 0;
    f.FrameUpdate();
    if (f.FadingOut || g_removedType != 0) fail = 5;

    // 5. no frames left while fading in: stop and remove the component
    f.FadingIn = true; f.FramesLeftForFade = 0;
    f.FrameUpdate();
    if (f.FadingIn || g_removedType != 0xf9) fail = 6;

    // 6. not fading: nothing happens beyond the dead-thing sweep
    int before = g_alphaCalls; int dead = g_removeDeadCalls;
    f.FadingIn = false; f.FadingOut = false; f.FramesLeftForFade = 5;
    f.FrameUpdate();
    if (g_alphaCalls != before || g_removeDeadCalls != dead + 1 || f.FramesLeftForFade != 5) fail = 7;

    // 7. parent lost its graphic appearance: clear both flags and remove the component
    g_removedType = 0; parent.PGraphicAppearance = 0; f.FadingIn = true; f.FadingOut = true;
    f.FrameUpdate();
    if (f.FadingIn || f.FadingOut || g_removedType != 0xf9 || g_removeDeadCalls != dead + 1) fail = 8;

    if (fail) { std::printf("006c3f40_TEST FAIL\n"); return 1; }
    std::printf("006c3f40_TEST PASS\n");
    return 0;
}
