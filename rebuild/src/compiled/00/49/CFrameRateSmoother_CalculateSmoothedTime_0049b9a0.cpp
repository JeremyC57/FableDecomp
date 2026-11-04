// CFrameRateSmoother::CalculateSmoothedTime() -- average the NoTimes ring entries starting at FirstTime into SmoothedTime
// Leaf: ?CalculateSmoothedTime@CFrameRateSmoother@@AAEXXZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe @ 0x0049B9A0, 59 bytes (16-aligned /O2 entry), thiscall `ret`.
// Identity: Ego_r.pdb `?CalculateSmoothedTime@CFrameRateSmoother@@AAEXXZ` (private); the catalog row name; only caller the
// landed CFrameRateSmoother::AddFrameTime 0x0049B9E0.
// Layout: Ego_r.pdb CFrameRateSmoother (STLport CArray 12 bytes): Times (CArray<float>) +0x08 (data pointer), MaxNoTimes
// +0x14, NoTimes +0x18, NoFramesToChangeFrameRateOver +0x1C, FirstTime +0x20, LastTime +0x24, SmoothedTime (double) +0x28.
// Bases (RTTI): CInitBaseClass (Valid +0x04) -> CBaseClass; the double member aligns the class to 8, Times at +8.
// Shape: a float accumulator from the 0.0f pool constant 0x0122DEDC; the ring index advances by `(i + 1) % MaxNoTimes`
// (`lea ; cdq ; idiv`); VC7.1 turns the counted loop into a down-counter on a copy of NoTimes; the average is
// `total / NoTimes` (fild of the count homed in the `push ecx` slot, then fdivr) stored as the double.
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
private:
    void CalculateSmoothedTime();
};
void CFrameRateSmoother::CalculateSmoothedTime()
{
    float total = 0.0f;
    long index = FirstTime;
    for (long i = 0; i < NoTimes; ++i) {
        total += Times[index];
        index = (index + 1) % MaxNoTimes;
    }
    SmoothedTime = total / NoTimes;
}