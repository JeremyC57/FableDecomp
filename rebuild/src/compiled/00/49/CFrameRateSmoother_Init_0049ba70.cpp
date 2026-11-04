// CFrameRateSmoother::Init(long, long) -- initialise the frame-time ring: base Initialise, Times sized to maxNoTimes, smoothed time 0.1, counters reset
// Leaf: ?Init@CFrameRateSmoother@@QAE_NJJ@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe @ 0x0049BA70, 85 bytes (16-aligned /O2 entry), thiscall `ret 8`, returns bool.
// Identity: Ego_r.pdb `?Init@CFrameRateSmoother@@QAE_NJJ@Z`: the body starts with CInitBaseClass::Initialise() 0x0099A350
// (RTTI: CFrameRateSmoother -> CInitBaseClass -> CBaseClass), resizes Times (+0x08), stores SmoothedTime (+0x28),
// MaxNoTimes (+0x14) = arg 1, NoFramesToChangeFrameRateOver (+0x1C) = arg 2 and clears NoTimes / FirstTime / LastTime --
// the same members CalculateSmoothedTime 0x0049B9A0 and the landed AddFrameTime 0x0049B9E0 use; caller 0x00418901 (the game
// component set-up). The catalog label CEditInputProcessMapPlacement::AddToInterface is propagated.
// Callees: CInitBaseClass::Initialise 0x0099A350 (protected, PDB `?Initialise@CInitBaseClass@@IAE_NXZ`);
// std::vector<float>::resize(size_type, const float&) 0x00442770 (STLport; reached through the inline resize(n), whose
// `_Tp()` temporary is the zeroed frame word).
// Shape: the success path is the if body (an early `return false` moves the failure block up); 0.1f widened at compile time (the double pool constant 0x01238B48 = 0x3FB99999A0000000).
#include "engine/CArray.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CInitBaseClass : public CBaseClass { public:
    bool Valid;                                      // +0x04
protected:
    bool Initialise();                               // 0x0099A350
};
class CFrameRateSmoother : public CInitBaseClass { public:
    CArray<float> Times;                             // +0x08
    long          MaxNoTimes;                        // +0x14
    long          NoTimes;                           // +0x18
    long          NoFramesToChangeFrameRateOver;     // +0x1C
    long          FirstTime;                         // +0x20
    long          LastTime;                          // +0x24
    double        SmoothedTime;                      // +0x28
    bool Init(long maxNoTimes, long noFramesToChangeFrameRateOver);
};
bool CFrameRateSmoother::Init(long maxNoTimes, long noFramesToChangeFrameRateOver)
{
    if (Initialise()) {
        Times.resize(maxNoTimes);
        SmoothedTime = 0.1f;
        MaxNoTimes = maxNoTimes;
        FirstTime = 0;
        LastTime = 0;
        NoTimes = 0;
        NoFramesToChangeFrameRateOver = noFramesToChangeFrameRateOver;
        return true;
    }
    return false;
}