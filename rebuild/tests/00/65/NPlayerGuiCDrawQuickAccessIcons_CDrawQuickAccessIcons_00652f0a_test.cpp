// Behaviour test for 00652f0a: body copied from src/00652f0a.cpp (CDrawQuickAccessIcons ctor), callees stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_baseCtor = 0, g_fadeCtor = 0, g_handleCtor = 0, g_resizeE = 0, g_resizeC = 0; static long g_resizeECount = -1; static unsigned long g_resizeCCount = 0;
static const void* g_baseGui = 0; static const void* g_baseList = 0;
class CPlayerGui;
class CBaseClass {
public:  virtual void v00() {} CBaseClass() { ++g_handleCtor; } };
class CIVCountedPointeeBase;
struct CIVCountedPointer { CIVCountedPointeeBase* Object; __forceinline CIVCountedPointer() : Object(0) { } };
class CEnginePrimitiveHandle : public CBaseClass { public: CIVCountedPointer Data; virtual void v00() {} __forceinline CEnginePrimitiveHandle() { } };
class C2DCoordI { public: long X; long Y; C2DCoordI() { } };
namespace NPlayerGui {
template <class P> struct CAllocProxy { P p; CAllocProxy(P v) : p(v) { } operator P() const { return p; } CAllocProxy& operator=(P v) { p = v; return *this; } };
struct CRetailHandleVector { void* m_pFirst; void* m_pLast; CAllocProxy<void*> m_pEnd; CRetailHandleVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { } };
struct CRetailEmitterVector { void* m_pFirst; void* m_pLast; CAllocProxy<void*> m_pEnd; CRetailEmitterVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { } void Resize(long count) { ++g_resizeE; g_resizeECount = count; } };
struct CRetailCoordVector { C2DCoordI* m_pFirst; C2DCoordI* m_pLast; CAllocProxy<C2DCoordI*> m_pEnd; CRetailCoordVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { } void resize(unsigned long count, const C2DCoordI& value) { ++g_resizeC; g_resizeCCount = count; } };
struct CFadeInFadeOutBase { unsigned char _body[0x1c]; CFadeInFadeOutBase() { ++g_fadeCtor; _body[0] = 3; } };
class CDrawBase { public: virtual void v00() {} unsigned char _body[0x28 - 4]; CDrawBase(CPlayerGui* gui, void* drawList) { ++g_baseCtor; g_baseGui = gui; g_baseList = drawList; } };
class CDrawQuickAccessIcons : public CDrawBase { public:
    virtual void v00() {}
    unsigned long ChargingSoundID; long ChargingButtonIndex; float ChargingButtonTime; bool Charging; bool FullyCharged; float FullyChargedTime; long FlashButton;
    CFadeInFadeOutBase FlashFade; CEnginePrimitiveHandle ChargeUpPrimitiveHandle; CRetailHandleVector SpecialPrimitiveHandles; CRetailHandleVector QuickAccessSpellCrossHandles;
    CRetailEmitterVector Effects; CRetailCoordVector EffectPositions;
    CDrawQuickAccessIcons(CPlayerGui* gui, void* drawList);
};
CDrawQuickAccessIcons::CDrawQuickAccessIcons(CPlayerGui* gui, void* drawList)
    : CDrawBase(gui, drawList), ChargingSoundID(0), ChargingButtonIndex(-1), ChargingButtonTime(0.0f), Charging(false),
      FullyCharged(false), FullyChargedTime(0.0f), FlashButton(0), FlashFade(), ChargeUpPrimitiveHandle(),
      SpecialPrimitiveHandles(), QuickAccessSpellCrossHandles(), Effects(), EffectPositions()
{
    Effects.Resize(13);
    EffectPositions.resize(13, C2DCoordI());
}
}
int main()
{
    unsigned char raw[sizeof(NPlayerGui::CDrawQuickAccessIcons)]; memset(raw, 0xAA, sizeof(raw)); int gui = 0, list = 0;
    NPlayerGui::CDrawQuickAccessIcons* d = new (raw) NPlayerGui::CDrawQuickAccessIcons((CPlayerGui*)&gui, &list);
    CHECK(g_baseCtor == 1 && g_baseGui == &gui && g_baseList == &list && g_fadeCtor == 1 && g_handleCtor == 1);
    CHECK(g_resizeE == 1 && g_resizeECount == 13 && g_resizeC == 1 && g_resizeCCount == 13);
    CHECK(d->ChargingSoundID == 0 && d->ChargingButtonIndex == -1 && d->ChargingButtonTime == 0.0f && d->Charging == false && d->FullyCharged == false && d->FullyChargedTime == 0.0f && d->FlashButton == 0);
    CHECK(d->FlashFade._body[0] == 3 && d->ChargeUpPrimitiveHandle.Data.Object == 0);
    CHECK(d->SpecialPrimitiveHandles.m_pFirst == 0 && d->SpecialPrimitiveHandles.m_pEnd.p == 0 && d->QuickAccessSpellCrossHandles.m_pLast == 0 && d->QuickAccessSpellCrossHandles.m_pEnd.p == 0);
    CHECK(d->Effects.m_pFirst == 0 && d->Effects.m_pEnd.p == 0 && d->EffectPositions.m_pFirst == 0 && d->EffectPositions.m_pLast == 0 && d->EffectPositions.m_pEnd.p == 0);
    if (fails == 0) printf("00652f0a_TEST PASS\n"); else printf("00652f0a_TEST FAIL\n");
    return fails;
}