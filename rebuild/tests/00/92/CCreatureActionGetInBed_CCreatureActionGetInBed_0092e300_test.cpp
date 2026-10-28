// Behaviour test: CCreatureAction_GetInBed(creature, bed, anim, sleep_anim_name, c, d) (0x0092E300) -- base action on
// the animation (no sound, flags 0/1), bed pointer, the animation's group name (CDefString -> CCharString through the
// def string table) and the sleep animation name copied; the bed animation fields start empty.
#include <cstdio>
#include <cstring>
static const char* g_table[] = { "", "BED_GROUP", "OTHER" };
class CAnimationEntry { public: int GroupName; };                  // CDefString: index into the def string table
struct CGetInBed {
    int Flag0, Flag1; const void* PBed; char AnimGroup[32]; char SleepAnimName[32]; char BedGetInAnim[32]; char BedSleepAnim[32];
    CGetInBed(const void* bed, const CAnimationEntry& anim, const char* sleep)
        : Flag0(0), Flag1(1), PBed(bed)
    {
        std::strcpy(AnimGroup, g_table[anim.GroupName]);
        std::strcpy(SleepAnimName, sleep);
        BedGetInAnim[0] = 0; BedSleepAnim[0] = 0;
    }
};
int main()
{
    int bed = 0;
    CAnimationEntry anim = { 1 };
    CGetInBed a(&bed, anim, "SLEEP_LOOP");
    if (!(a.PBed == &bed && std::strcmp(a.AnimGroup, "BED_GROUP") == 0 && std::strcmp(a.SleepAnimName, "SLEEP_LOOP") == 0
          && a.BedGetInAnim[0] == 0 && a.BedSleepAnim[0] == 0 && a.Flag0 == 0 && a.Flag1 == 1)) {
        std::printf("0092e300_TEST FAIL\n"); return 1;
    }
    std::printf("0092e300_TEST PASS\n");
    return 0;
}