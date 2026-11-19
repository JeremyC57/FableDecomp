// Behaviour test for 0x00754D80 std::for_each<pair<float,const CAnimationEntry*>*, CGetBestTurningAnim>: the row's includes and body
// verbatim. The functor state is set and read through its 12-byte image (members private, as in the PDB). The first entry is taken
// whatever its angle; later entries replace the best only when strictly closer (|angle - rotation|) to the wanted rotation; an empty
// range returns the functor unchanged; negative differences use the absolute value.
#include <stdio.h>
#include <string.h>
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

struct SImage { float RotationAngle; float BestMatchAngle; const CAnimationEntry* PBestMatchAnim; };
static CGetBestTurningAnim Make(float rot) { SImage s = { rot, 0.0f, 0 }; CGetBestTurningAnim f; memcpy(&f, &s, sizeof(s)); return f; }
static SImage Image(const CGetBestTurningAnim& f) { SImage s; memcpy(&s, &f, sizeof(s)); return s; }
static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
int main()
{
    char a[6];
    const CAnimationEntry* A = (const CAnimationEntry*)&a[0]; const CAnimationEntry* B = (const CAnimationEntry*)&a[1];
    const CAnimationEntry* C = (const CAnimationEntry*)&a[2]; const CAnimationEntry* D = (const CAnimationEntry*)&a[3];
    std::pair<float, const CAnimationEntry*> e[5];
    e[0].first = 2.0f;  e[0].second = A;    // far, but first: always taken
    e[1].first = 0.75f; e[1].second = B;    // |0.25 - 0.75| = 0.5
    e[2].first = 0.5f;  e[2].second = C;    // 0.25: closer
    e[3].first = 0.0f;  e[3].second = D;    // |-0.25| = 0.25: a tie, not strictly closer -> C kept
    e[4].first = 0.5f;  e[4].second = A;    // equal again -> C kept
    SImage r = Image(std::for_each(e, e + 5, Make(0.25f)));
    check(r.PBestMatchAnim == C && r.BestMatchAngle == 0.5f, "closest, strict, first of equals");
    check(r.RotationAngle == 0.25f, "rotation untouched");
    r = Image(std::for_each(e, e + 1, Make(0.25f)));
    check(r.PBestMatchAnim == A && r.BestMatchAngle == 2.0f, "first entry taken whatever its distance");
    r = Image(std::for_each(e + 3, e + 4, Make(0.25f)));
    check(r.PBestMatchAnim == D && r.BestMatchAngle == 0.0f, "single entry");
    r = Image(std::for_each(e, e, Make(0.25f)));
    check(r.PBestMatchAnim == 0 && r.BestMatchAngle == 0.0f, "empty range");
    e[1].first = -1.0f;                      // rotation -0.9: |-1 - -0.9| = 0.1 beats |2 + 0.9|
    r = Image(std::for_each(e, e + 2, Make(-0.9f)));
    check(r.PBestMatchAnim == B && r.BestMatchAngle == -1.0f, "negative angles by absolute difference");
    e[1].first = -3.0f;                      // |-3 + 0.9| = 2.1 v |2 + 0.9| = 2.9: closer although further below
    r = Image(std::for_each(e, e + 2, Make(-0.9f)));
    check(r.PBestMatchAnim == B, "absolute value on the below side");
    if (!g_fail) printf("00754d80_TEST PASS\n");
    return g_fail;
}