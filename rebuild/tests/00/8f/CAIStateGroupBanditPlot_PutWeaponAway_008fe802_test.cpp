// Behaviour test for 0x008FE802 CAIStateGroup_BanditPlot::PutWeaponAway (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_BANDITPLOT_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE_VIRTUAL_MODEL
#include "engine/CAIStateGroup_BanditPlot.h"
#include <stdio.h>
// PutWeaponAway is protected: a scratch accessor struct derived from CAIStateGroup_BanditPlot names it (no body of CAIStateGroup_BanditPlot, nothing instantiated).
struct NF429Access : CAIStateGroup_BanditPlot {
    static bool call(CAIStateGroup_BanditPlot* p) { return static_cast<NF429Access*>(p)->CAIStateGroup_BanditPlot::PutWeaponAway(); }
};
static unsigned char g_obj[sizeof(CAIStateGroup_BanditPlot)];
int main(void)
{
    CAIStateGroup_BanditPlot* p = (CAIStateGroup_BanditPlot*)g_obj;
    bool r = NF429Access::call(p);
    int ok = (r == false);
    if (ok) { printf("AUTO_TINY_008fe802_TEST PASS\n"); return 0; }
    printf("FAIL_008fe802\n");
    return 1;
}
