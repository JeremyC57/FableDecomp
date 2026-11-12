// Behaviour test for 0x00658A35 CTCVillage::IsSchoolTime() const (standalone model): IsTimeBetween gets the def's SchoolStartTimes
// (+0x54) and SchoolDuration (+0x60); both by-value def references taken for the call are released afterwards.
#include <cstdio>
struct CVillageDefModel {
    void** vtbl;
    long   refs;                               // +0x04
    char   pad[0x54 - 0x08];
    long   SchoolStartTimes[3];                  // +0x54 (STLport vector<long>: start, finish, end_of_storage)
    long   SchoolDuration;                       // +0x60
};
struct DefPtr {
    CVillageDefModel* o;
    DefPtr(CVillageDefModel* d) : o(d) { if (o) ++o->refs; }
    ~DefPtr() { if (o) --o->refs; }
    CVillageDefModel* operator->() const { return o; }
};
static const void* g_times = 0;
static long g_duration = 0;
struct CTCVillageModel {
    char pad[0x94];
    CVillageDefModel* PDef;                    // +0x94
    DefPtr GetPDef() const { return DefPtr(PDef); }
    bool IsTimeBetween(const long* times, long duration) const { g_times = times; g_duration = duration; return duration > 0; }
    bool IsSchoolTime() const { return IsTimeBetween(GetPDef()->SchoolStartTimes, GetPDef()->SchoolDuration); }
};
int main()
{
    CVillageDefModel def;
    def.refs = 3;
    def.SchoolDuration = 1234;
    CTCVillageModel village;
    village.PDef = &def;
    bool r = village.IsSchoolTime();
    if (!r || g_duration != 1234 || g_times != (const void*)def.SchoolStartTimes || def.refs != 3
        || (char*)def.SchoolStartTimes - (char*)&def != 0x54 || (char*)&def.SchoolDuration - (char*)&def != 0x60) {
        std::printf("CTCVillage_00658a35_TEST FAIL\n");
        return 1;
    }
    std::printf("CTCVillage_00658a35_TEST PASS\n");
    return 0;
}
