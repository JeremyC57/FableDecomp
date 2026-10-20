#include <cstdio>
struct CTCVillage { bool schoolTime; bool IsTimeToGoToSchool(void*, void*, bool) { return schoolTime; } };
struct CTCVillager { CTCVillage* village; };
static CTCVillage* GFGetPVillageInterface(CTCVillager* v) { return v ? v->village : 0; }
struct Material { unsigned long Flags; int refs; };
struct Thing { bool hasVillager; CTCVillager villager; Thing* school; Material* material; };
static bool IsRunnable(Thing* kid)
{
    Thing* school = kid->school;
    if (school && kid->hasVillager) {
        if (GFGetPVillageInterface(&kid->villager)) {
            Material* m = school->material;
            if (m) {
                ++m->refs;
                bool result = false;
                if ((m->Flags & 4) && school->hasVillager && GFGetPVillageInterface(&school->villager))
                    result = GFGetPVillageInterface(&school->villager)->IsTimeToGoToSchool(kid, 0, false);
                --m->refs;
                return result;
            }
        }
    }
    return false;
}
int main()
{
    CTCVillage village = { true };
    Material mat = { 4, 1 };
    Thing school = { true, { &village }, 0, &mat };
    Thing kid = { true, { &village }, &school, 0 };
    if (!IsRunnable(&kid) || mat.refs != 1) { std::printf("00942a00_TEST FAIL\n"); return 1; }
    village.schoolTime = false;
    if (IsRunnable(&kid)) { std::printf("00942a00_TEST FAIL\n"); return 1; }
    village.schoolTime = true; mat.Flags = 0;
    if (IsRunnable(&kid)) { std::printf("00942a00_TEST FAIL\n"); return 1; }
    mat.Flags = 4; kid.school = 0;
    if (IsRunnable(&kid)) { std::printf("00942a00_TEST FAIL\n"); return 1; }
    std::printf("00942a00_TEST PASS\n");
    return 0;
}
