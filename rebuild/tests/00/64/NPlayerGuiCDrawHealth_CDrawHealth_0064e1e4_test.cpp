#include <cstdio>
// Behaviour test for 0064e1e4.

struct CPlayerGui { int id; };
struct CRetailHandleVector { unsigned int reserved; CRetailHandleVector() : reserved(0) { } void Resize(long n) { reserved = (unsigned int)n; } };
struct CRetailEmitterVector { void* m_pFirst; void* m_pLast; void* m_pEnd; unsigned int reserved;
    CRetailEmitterVector() : m_pFirst(0), m_pLast(0), m_pEnd(0), reserved(0) { } void Resize(long n) { reserved = (unsigned int)n; } };
struct CFadeInFadeOutBase { int built; CFadeInFadeOutBase() : built(1) { } };
struct CDrawBase {
    CPlayerGui* PlayerGui; void* DrawList; CRetailHandleVector PrimitiveHandles;
    CDrawBase(CPlayerGui* gui, void* list) : PlayerGui(gui), DrawList(list) { }
    virtual ~CDrawBase() { }
};

// NPlayerGui::CDrawHealth::CDrawHealth: base built, empty effects, previous health -1, fade built, zeroed reduction/flash, pulse 1,
// two effect slots and twelve primitive handles reserved.
struct CDrawHealth : CDrawBase {
    CRetailEmitterVector Effects; float PreviousHealth; CFadeInFadeOutBase HealthIncreaseFade;
    float HealthReduction, HealthReductionAlpha, HealthFlashTimer, LowHealthPulseScale, LowHealthPulseDirection; long PreviousEffect;
    CDrawHealth(CPlayerGui* gui, void* drawList)
        : CDrawBase(gui, drawList), Effects(), PreviousHealth(-1.0f), HealthIncreaseFade(), HealthReduction(0.0f),
          HealthReductionAlpha(0.0f), HealthFlashTimer(0.0f), LowHealthPulseScale(1.0f), LowHealthPulseDirection(1.0f), PreviousEffect(0)
    { Effects.Resize(2); PrimitiveHandles.Resize(12); }
};
int main()
{
    CPlayerGui gui; int list = 0;
    CDrawHealth h(&gui, &list);
    int ok = h.PlayerGui == &gui && h.PreviousHealth == -1.0f && h.HealthIncreaseFade.built && h.HealthReduction == 0.0f
          && h.LowHealthPulseScale == 1.0f && h.LowHealthPulseDirection == 1.0f && h.PreviousEffect == 0 && h.Effects.reserved == 2
          && h.PrimitiveHandles.reserved == 12 && h.Effects.m_pFirst == 0;
    if (!ok) { std::printf("0064e1e4_TEST FAIL\n"); return 1; }
    std::printf("0064e1e4_TEST PASS\n");
    return 0;
}