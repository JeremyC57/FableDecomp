// Behaviour test for 0x005305A0 (source body copied from src/005305a0.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour() { }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
    CRGBColour& operator+=(const CRGBColour& o) { R += o.R; G += o.G; B += o.B; A += o.A; return *this; }
};

namespace NUISystem {

class CComponent {
public:
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void ChangeColour(const CRGBColour& colour, float time, bool flags);        // slot 38 (+0x98)
    virtual void ChangeColourDelta(const CRGBColour& delta, float time, bool flags);    // slot 39
    unsigned char _pad_0x04[0x80];          // +0x04
    CRGBColour    Colour;                   // +0x84
};

void CComponent::ChangeColourDelta(const CRGBColour& delta, float time, bool flags)
{
    CRGBColour colour = Colour;
    colour += delta;
    ChangeColour(colour, time, flags);
}

}

// ---- test harness ----
void NUISystem::CComponent::v000() { }
void NUISystem::CComponent::v001() { }
void NUISystem::CComponent::v002() { }
void NUISystem::CComponent::v003() { }
void NUISystem::CComponent::v004() { }
void NUISystem::CComponent::v005() { }
void NUISystem::CComponent::v006() { }
void NUISystem::CComponent::v007() { }
void NUISystem::CComponent::v008() { }
void NUISystem::CComponent::v009() { }
void NUISystem::CComponent::v010() { }
void NUISystem::CComponent::v011() { }
void NUISystem::CComponent::v012() { }
void NUISystem::CComponent::v013() { }
void NUISystem::CComponent::v014() { }
void NUISystem::CComponent::v015() { }
void NUISystem::CComponent::v016() { }
void NUISystem::CComponent::v017() { }
void NUISystem::CComponent::v018() { }
void NUISystem::CComponent::v019() { }
void NUISystem::CComponent::v020() { }
void NUISystem::CComponent::v021() { }
void NUISystem::CComponent::v022() { }
void NUISystem::CComponent::v023() { }
void NUISystem::CComponent::v024() { }
void NUISystem::CComponent::v025() { }
void NUISystem::CComponent::v026() { }
void NUISystem::CComponent::v027() { }
void NUISystem::CComponent::v028() { }
void NUISystem::CComponent::v029() { }
void NUISystem::CComponent::v030() { }
void NUISystem::CComponent::v031() { }
void NUISystem::CComponent::v032() { }
void NUISystem::CComponent::v033() { }
void NUISystem::CComponent::v034() { }
void NUISystem::CComponent::v035() { }
void NUISystem::CComponent::v036() { }
void NUISystem::CComponent::v037() { }
static CRGBColour g_seen; static float g_time = 0; static int g_flags = -1, g_calls = 0;
void NUISystem::CComponent::ChangeColour(const CRGBColour& colour, float time, bool flags)
{
    ++g_calls; g_seen.B = colour.B; g_seen.G = colour.G; g_seen.R = colour.R; g_seen.A = colour.A; g_time = time; g_flags = flags ? 1 : 0;
}
int main()
{
    NUISystem::CComponent c;
    memset(c._pad_0x04, 0, sizeof c._pad_0x04);
    c.Colour.B = 10; c.Colour.G = 20; c.Colour.R = 200; c.Colour.A = 255;
    CRGBColour delta; delta.B = 5; delta.G = 250; delta.R = 100; delta.A = 1;
    c.ChangeColourDelta(delta, 0.5f, true);
    int ok = 1;
    if (g_calls != 1 || g_seen.B != 15 || g_seen.G != 14 || g_seen.R != 44 || g_seen.A != 0 || g_flags != 1) ok = 0;
    float dt = g_time - 0.5f; if (dt < 0) dt = -dt; if (dt > 0.0001f) ok = 0;
    if (c.Colour.B != 10 || c.Colour.G != 20 || c.Colour.R != 200 || c.Colour.A != 255) ok = 0;   // the current colour is untouched
    printf("005305a0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}