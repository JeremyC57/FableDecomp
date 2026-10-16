// Behaviour test for CTCTavernGameSpotTheAddition::CTCTavernGameSpotTheAddition (0x007D8DA0): empty knick-knack vector,
// TargetTime = the last prize score, Pointer/PointerScale cleared, def reference released.
#include <cstdio>
struct Prize { float Score; long Mult; };
struct Def { long count; int destroyed; Prize p[3]; };
struct Spot {
    void* KFirst; void* KLast; void* KEnd; float TargetTime; float PointerScale; void* Pointer;
    Spot(Def& d) : KFirst(0), KLast(0), KEnd(0)
    {
        ++d.count;
        TargetTime = (d.p + 3 - 1)->Score;
        PointerScale = 0.0f; Pointer = 0;
        if (--d.count == 0) d.destroyed = 1;
    }
};
int main()
{
    Def d; d.count = 0; d.destroyed = 0; d.p[2].Score = 45;
    Spot s(d);
    if (s.TargetTime != 45 || s.Pointer || s.PointerScale != 0 || s.KFirst || !d.destroyed) { std::printf("007d8da0_TEST FAIL\n"); return 1; }
    std::printf("007d8da0_TEST PASS\n");
    return 0;
}