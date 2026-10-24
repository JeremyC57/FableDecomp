#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __cdecl
#endif
static int g_extCopies = 0;   // out-of-line copy constructors the retail body calls
static int g_links = 0;       // CBaseIntelligentPointer::SetPItem calls
namespace N0069c860 {
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }   // user memberwise copy: fixed-offset dword moves, no block copy
};
struct CQuake {                                  // CGameCameraManager::CQuake (PDB, sizeof 0x30)
    C3DVector Position;                          // +0x00
    C3DVector Axis;                              // +0x0C
    long      Frame;                             // +0x18
    float     Amplitude;                         // +0x1C
    float     Duration;                          // +0x20
    float     Frequency;                         // +0x24
    float     Phase;                             // +0x28
    bool      Global;                            // +0x2C
    CQuake() { }
    __forceinline CQuake(const CQuake& o)        // the implicit memberwise copy, spelled out so VC7.1 inlines it into _Construct
        : Position(o.Position), Axis(o.Axis), Frame(o.Frame), Amplitude(o.Amplitude), Duration(o.Duration),
          Frequency(o.Frequency), Phase(o.Phase), Global(o.Global) { }
};

void __fastcall _Construct(CQuake* p, const CQuake& value)
{
    if (p != 0)
        new (p) CQuake(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N0069c860;
    CQuake src;
    src.Position.X = 1.0f;
    src.Position.Y = 2.0f;
    src.Position.Z = 3.0f;
    src.Axis.X = 0.0f;
    src.Axis.Y = 0.0f;
    src.Axis.Z = 1.0f;
    src.Frame = 17;
    src.Amplitude = 0.5f;
    src.Duration = 2.0f;
    src.Frequency = 8.0f;
    src.Phase = 0.25f;
    src.Global = true;
    CQuake* slot = reinterpret_cast<CQuake*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 0 && g_links == 0;
    if (!(slot->Position.X == src.Position.X)) ok = false;
    if (!(slot->Position.Y == src.Position.Y)) ok = false;
    if (!(slot->Position.Z == src.Position.Z)) ok = false;
    if (!(slot->Axis.X == src.Axis.X)) ok = false;
    if (!(slot->Axis.Y == src.Axis.Y)) ok = false;
    if (!(slot->Axis.Z == src.Axis.Z)) ok = false;
    if (!(slot->Frame == src.Frame)) ok = false;
    if (!(slot->Amplitude == src.Amplitude)) ok = false;
    if (!(slot->Duration == src.Duration)) ok = false;
    if (!(slot->Frequency == src.Frequency)) ok = false;
    if (!(slot->Phase == src.Phase)) ok = false;
    if (!(slot->Global == src.Global)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 0 || g_links != 0) ok = false;
    if (!ok) { std::printf("0069c860_TEST FAIL\n"); return 1; }
    std::printf("0069c860_TEST PASS\n");
    return 0;
}