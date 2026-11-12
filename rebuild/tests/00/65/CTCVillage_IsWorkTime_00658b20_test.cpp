// Behaviour test for 0x00658B20 CTCVillage::IsWorkTime() const (standalone model): IsTimeBetween gets the def's WorkStartTimes
// (+0x64) and WorkDuration (+0x70); both by-value def references taken for the call are released afterwards.
#include <cstdio>
struct CVillageDefModel {
    void** vtbl;
    long   refs;                               // +0x04
    char   pad[0x64 - 0x08];
    long   WorkStartTimes[3];                  // +0x64 (STLport vector<long>: start, finish, end_of_storage)
    long   WorkDuration;                       // +0x70
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
    bool IsWorkTime() const { return IsTimeBetween(GetPDef()->WorkStartTimes, GetPDef()->WorkDuration); }
};
int main()
{
    CVillageDefModel def;
    def.refs = 3;
    def.WorkDuration = 1234;
    CTCVillageModel village;
    village.PDef = &def;
    bool r = village.IsWorkTime();
    if (!r || g_duration != 1234 || g_times != (const void*)def.WorkStartTimes || def.refs != 3) {
        std::printf("CTCVillage_00658b20_TEST FAIL\n");
        return 1;
    }
    std::printf("CTCVillage_00658b20_TEST PASS\n");
    return 0;
}
