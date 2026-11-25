// Behaviour test for 0x00918ED0 CAIStateGroup_Gaze::InsideAtNight (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_GAZE
#define FABLE_RETAIL_CAISTATEGROUP_GAZE_RTTI_DECLS
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_Gaze.h"
#include <stdio.h>
// InsideAtNight is protected: a scratch accessor struct derived from CAIStateGroup_Gaze names it (no body of CAIStateGroup_Gaze, nothing instantiated).
struct NF429Access : CAIStateGroup_Gaze {
    static bool call(CAIStateGroup_Gaze* p) { return static_cast<NF429Access*>(p)->CAIStateGroup_Gaze::InsideAtNight(); }
};
static unsigned char g_obj[sizeof(CAIStateGroup_Gaze)];
int main(void)
{
    CAIStateGroup_Gaze* p = (CAIStateGroup_Gaze*)g_obj;
    bool r = NF429Access::call(p);
    int ok = (r == false);
    if (ok) { printf("AUTO_TINY_00918ed0_TEST PASS\n"); return 0; }
    printf("FAIL_00918ed0\n");
    return 1;
}
