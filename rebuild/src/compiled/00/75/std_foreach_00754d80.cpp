// std::for_each(std::pair<float, const CAnimationEntry*>* first, std::pair<float, const CAnimationEntry*>* last, CGetBestTurningAnim f) -- picks the turning animation whose angle is closest to the wanted rotation
// Leaf: ??$for_each@PAU?$pair@MPBVCAnimationEntry@@@std@@VCGetBestTurningAnim@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00754D80, 92 bytes, /Gr `__fastcall` (hidden functor result ecx, first edx, last and the
//   12-byte functor on the stack), `ret 10h`. Caller: CTCLook 0x00753743.
// Identity: tc_look.obj (Ego_r #65 / FableWin #161 `std::_For_each<std::pair<float,CAnimationEntry const *> *,`anonymous
//   namespace'::CGetBestTurningAnim>`, the VC10 name of the same instantiation); retail is STLport 4.6 for_each over the vector's raw
//   pointers (shared stlport/fable_algorithm.h FABLE_RETAIL_STLP_FOR_EACH_BODY).
// Functor: tc_look.cpp's anonymous-namespace CGetBestTurningAnim (Ego_d class record: RotationAngle +0, BestMatchAngle +4,
//   PBestMatchAnim +8, all private; no shared header -- the class is local to tc_look.cpp). operator() in FableWin 0x025B7AE0 order with
//   its locals `bloke`, `this_entry`, `best_entry` (float); the comparison uses the C++ float overload fabs(float) (FableWin ?fabs@@YAMM@Z).
#include <math.h>
#define FABLE_RETAIL_STLP_FOR_EACH_BODY
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
class CAnimationEntry;
namespace {
class CGetBestTurningAnim {
public:
    void operator()(const std::pair<float, const CAnimationEntry*>& bloke)
    {
        if (!PBestMatchAnim) {
            BestMatchAngle = bloke.first;
            PBestMatchAnim = bloke.second;
        } else {
            float this_entry = bloke.first - RotationAngle;
            float best_entry = BestMatchAngle - RotationAngle;
            if (fabs(this_entry) < fabs(best_entry)) {
                BestMatchAngle = bloke.first;
                PBestMatchAnim = bloke.second;
            }
        }
    }
private:
    float RotationAngle;                               // +0x00
    float BestMatchAngle;                              // +0x04
    const CAnimationEntry* PBestMatchAnim;             // +0x08
};
}
template CGetBestTurningAnim FABLE_FASTCALL std::for_each(std::pair<float, const CAnimationEntry*>*, std::pair<float, const CAnimationEntry*>*, CGetBestTurningAnim);