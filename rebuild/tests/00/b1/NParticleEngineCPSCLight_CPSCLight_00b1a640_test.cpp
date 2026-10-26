// Behaviour test for 00b1a640: src/00b1a640.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. NParticleEngine::CPSCLight::CPSCLight()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// NParticleEngine::CPSCLight::CPSCLight() -- particle light component: CPSCBase base, lifetime/respawn timers and radii, the light flags (enabled + start colour), black fade colour, white start/mid/end colours, an empty primitive handle and an emptied user-point array
// Leaf: ??0CPSCLight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B1A640, length 192 bytes. Vtable 0x012A0624 (RTTI NParticleEngine::CPSCLight).
// Callees: NParticleEngine::CPSCBase::CPSCBase @ 0x00AF2620, CBaseClass::CBaseClass @ 0x0099A2D0 (the handle's base),
//          SGI __copy<12-byte> @ 0x0074CA20 (the array's clear(): erase(begin, end) = `last = copy(end, end, begin)`,
//          __fastcall with the iterator tag by reference and a null Distance*).
// Layout: Ego_r.pdb NParticleEngine::CPSCLight (0x70 bytes) -- the retail offsets equal the PDB ones; the
// handle is CEnginePrimitiveHandle (vtable 0x0122F598: CBaseClass + a counted Data pointer), the array the
// engine's 12-byte CArray<C3DVector>.
// Shape: member initialisers in declaration order. The eight one-bit flags at +0x48 are all initialised,
// so they fold into a single byte store (0x24 = LightEnabled | LightUseStartColour); the three at +0x49
// share the byte with unnamed bits, hence the `and 0F8h`. CRGBColour's (r,g,b,a) ctor stores R, G, B, A
// into a B,G,R,A byte layout. The array constructor zeroes the three pointers and then clears; its third word
// is STLport's alloc proxy (a member with its own constructor): with three plain pointers VC7.1 forwards the
// zero stores into the __copy arguments, the proxy makes it re-load [esi+64h]/[esi+60h] as retail.
class CBaseClass {
public:
    CBaseClass();                                  // 0x0099A2D0
    virtual void v000();
};

class CEnginePersistentPrimitiveBase;
struct CIVCountedPointer { CEnginePersistentPrimitiveBase* Object; CIVCountedPointer() : Object(0) { } };

class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer Data;                        // +0x04
    CEnginePrimitiveHandle() { }
    virtual void v001();
};

class CRGBColour { public:                               // B,G,R,A bytes over a packed 32-bit colour (4-aligned)
    union {
        struct { unsigned char B, G, R, A; };
        unsigned long Colour;
    };
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

class C3DVector { public: float X, Y, Z; };

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

C3DVector* __fastcall Copy_SElement12(C3DVector* first, C3DVector* last, C3DVector* result,
                                      const SRandomAccessIteratorTag& category, long* distance);   // 0x0074CA20

template<class T> struct SAllocProxy { T* _M_data; SAllocProxy(T* p) : _M_data(p) { } };
template<class T> class CArray { public:                  // the engine's 12-byte vector
    T* first; T* last; SAllocProxy<T> end_of_storage;
    void erase(T* f, T* l) { last = Copy_SElement12(l, last, f, SRandomAccessIteratorTag(), (long*)0); }
    void clear() { erase(first, last); }
    CArray() : first(0), last(0), end_of_storage(0) { }
};

namespace NParticleEngine {
class CParticleSystem;

class CPSCBase { public:                                  // vtable 0x0129F3F4
    virtual void v000();
    CParticleSystem* ParticleSystem;               // +0x04
    bool             UpdateStarted : 1;            // +0x08 bit 0
    bool             Initialised : 1;              // +0x08 bit 1
    CPSCBase();                                    // 0x00AF2620
};

class CPSCLight : public CPSCBase { public:                      // vtable 0x012A0624
    long          LightLifeTimer;                  // +0x0c
    long          LightRespawnDelayTimer;          // +0x10
    bool          LightIsAlive;                    // +0x14
    unsigned long LightPositionParam;              // +0x18
    float         LightLifeSecs;                   // +0x1c
    float         LightRespawnDelaySecs;           // +0x20
    float         LightStartTime;                  // +0x24
    float         LightTimelineSecs;               // +0x28
    float         LightStartRenderWorldRadius;     // +0x2c
    float         LightEndRenderWorldRadius;       // +0x30
    long          LightAttenuationFactor;          // +0x34
    long          LightFadeInEndInteger;           // +0x38
    long          LightFadeOutBeginInteger;        // +0x3c
    float         LightWorldRadiusFadeMinimum;     // +0x40
    CRGBColour    LightColourFadeMinimum;          // +0x44
    bool          LightWorldRadiusFadeEnable : 1;  // +0x48 bit 0
    bool          LightUseLifeSecs : 1;            // +0x48 bit 1
    bool          LightEnabled : 1;                // +0x48 bit 2
    bool          LightRespawns : 1;               // +0x48 bit 3
    bool          LightColourFadeEnable : 1;       // +0x48 bit 4
    bool          LightUseStartColour : 1;         // +0x48 bit 5
    bool          LightUseMidColour : 1;           // +0x48 bit 6
    bool          LightUseEndColour : 1;           // +0x48 bit 7
    bool          LightUseFadeColour : 1;          // +0x49 bit 0
    bool          LightUseTimelineSecs : 1;        // +0x49 bit 1
    bool          StartedTimer : 1;                // +0x49 bit 2
    CRGBColour    LightStartColour;                // +0x4c
    CRGBColour    LightMidColour;                  // +0x50
    CRGBColour    LightEndColour;                  // +0x54
    CEnginePrimitiveHandle PrimHandle;             // +0x58
    CArray<C3DVector>      LightUserPointsArray;   // +0x60
    CPSCLight();
};

CPSCLight::CPSCLight()
    : LightLifeTimer(0)
    , LightRespawnDelayTimer(0)
    , LightIsAlive(false)
    , LightLifeSecs(5.0f)
    , LightRespawnDelaySecs(0.0f)
    , LightStartTime(0.0f)
    , LightTimelineSecs(0.0f)
    , LightStartRenderWorldRadius(1.0f)
    , LightEndRenderWorldRadius(1.0f)
    , LightAttenuationFactor(0)
    , LightFadeInEndInteger(0)
    , LightFadeOutBeginInteger(1000)
    , LightWorldRadiusFadeMinimum(0.0f)
    , LightColourFadeMinimum(0, 0, 0, 0xff)
    , LightWorldRadiusFadeEnable(false)
    , LightUseLifeSecs(false)
    , LightEnabled(true)
    , LightRespawns(false)
    , LightColourFadeEnable(false)
    , LightUseStartColour(true)
    , LightUseMidColour(false)
    , LightUseEndColour(false)
    , LightUseFadeColour(false)
    , LightUseTimelineSecs(false)
    , StartedTimer(false)
    , LightStartColour(0xff, 0xff, 0xff, 0xff)
    , LightMidColour(0xff, 0xff, 0xff, 0xff)
    , LightEndColour(0xff, 0xff, 0xff, 0xff)
{
    LightUserPointsArray.clear();
}
}

// ---- stubs and driver ----
void CBaseClass::v000() {}
void CEnginePrimitiveHandle::v001() {}
void NParticleEngine::CPSCBase::v000() {}
inline void* operator new(unsigned int, void* p) { return p; }
static char g_log[32]; static int g_logn = 0;
static void Log(char c) { if (g_logn < 31) { g_log[g_logn++] = c; g_log[g_logn] = 0; } }
CBaseClass::CBaseClass() { Log('b'); }
NParticleEngine::CPSCBase::CPSCBase() : ParticleSystem(0) { UpdateStarted = false; Initialised = false; Log('B'); }
static C3DVector *g_cf, *g_cl, *g_cr; static long* g_cd; static int g_copies = 0;
C3DVector* __fastcall Copy_SElement12(C3DVector* first, C3DVector* last, C3DVector* result, const SRandomAccessIteratorTag&, long* distance)
{ g_cf = first; g_cl = last; g_cr = result; g_cd = distance; ++g_copies; Log('c'); return result + (last - first); }
int main()
{
    using namespace NParticleEngine;
    static unsigned char store[sizeof(CPSCLight)]; memset(store, 0xcd, sizeof store);
    CPSCLight* l = new (store) CPSCLight;
    CHECK(strcmp(g_log, "Bbc") == 0); CHECK(g_copies == 1);
    CHECK(g_cf == 0); CHECK(g_cl == 0); CHECK(g_cr == 0); CHECK(g_cd == 0);
    CHECK(l->LightUserPointsArray.first == 0); CHECK(l->LightUserPointsArray.last == 0); CHECK(l->LightUserPointsArray.end_of_storage._M_data == 0);
    CHECK(l->PrimHandle.Data.Object == 0);
    CHECK(l->LightLifeTimer == 0); CHECK(l->LightRespawnDelayTimer == 0); CHECK(l->LightIsAlive == false);
    CHECK(l->LightLifeSecs == 5.0f); CHECK(l->LightStartRenderWorldRadius == 1.0f); CHECK(l->LightEndRenderWorldRadius == 1.0f);
    CHECK(l->LightFadeOutBeginInteger == 1000); CHECK(l->LightColourFadeMinimum.A == 0xff); CHECK(l->LightColourFadeMinimum.R == 0);
    CHECK(l->LightEnabled); CHECK(l->LightUseStartColour); CHECK(!l->LightUseMidColour); CHECK(!l->LightRespawns); CHECK(!l->StartedTimer);
    CHECK(l->LightStartColour.Colour == 0xffffffff); CHECK(l->LightMidColour.Colour == 0xffffffff); CHECK(l->LightEndColour.Colour == 0xffffffff);
    if (fails == 0) printf("00b1a640_TEST PASS\n"); else printf("00b1a640_TEST FAIL\n");
    return fails;
}