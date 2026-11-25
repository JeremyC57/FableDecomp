// Behaviour test for 0x006D6570 CTCHeroExperience::GetExperiencePickUpMultiplier() const (NF446). The row is a pick-any re-row of the shared
// header's inline member, so the test uses the row's prelude verbatim (no class body of its own) and checks the header arithmetic, which is
// retail's: ExperienceMultiplierParam (+0x30) / PDef (+0x54) ->ExperienceMultiplierParamPerUnitComboMultiplier (+0x64), plus the double 1.0 at
// 0x01237F00, truncated, never below 1. The component and the def are raw buffers.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_STATEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_PICKUP_MULTIPLIER
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/EHeroExperienceStatCategory.h"
#include "engine/CThing.h"
#include "engine/CHeroExperienceDef.h"
#include "engine/CTCHeroExperience.h"
#include <stdio.h>
#include <string.h>
static unsigned char g_tc[0x100];
static unsigned char g_def[0x100];
static long run(float param, float per_unit)
{
    memset(g_tc, 0, sizeof g_tc); memset(g_def, 0, sizeof g_def);
    *(float*)(g_def + 0x64) = per_unit;
    *(float*)(g_tc + 0x30) = param;
    *(void**)(g_tc + 0x54) = g_def;
    return ((const CTCHeroExperience*)g_tc)->GetExperiencePickUpMultiplier();
}
int main()
{
    if (run(10.0f, 2.0f) != 6 || run(1.0f, 100.0f) != 1 || run(-50.0f, 1.0f) != 1 || run(0.5f, 0.25f) != 3 || run(7.9f, 1.0f) != 8) {
        printf("FAIL\n"); return 1;
    }
    printf("CTCHeroExperience_006d6570_TEST PASS\n");
    return 0;
}
