// Behaviour test for 0x00658A80 CTCVillage::IsStoryTime() const (standalone model): IsTimeBetween gets the def's StoryStartTimes
// (+0x44) and StoryDuration (+0x50); both by-value def references taken for the call are released afterwards.
#include <cstdio>
struct CVillageDefModel {
    void** vtbl;
    long   refs;                               // +0x04
    char   pad[0x44 - 0x08];
    long   StoryStartTimes[3];                  // +0x44 (STLport vector<long>: start, finish, end_of_storage)
    long   StoryDuration;                       // +0x50
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
    bool IsStoryTime() const { return IsTimeBetween(GetPDef()->StoryStartTimes, GetPDef()->StoryDuration); }
};
int main()
{
    CVillageDefModel def;
    def.refs = 3;
    def.StoryDuration = 1234;
    CTCVillageModel village;
    village.PDef = &def;
    bool r = village.IsStoryTime();
    if (!r || g_duration != 1234 || g_times != (const void*)def.StoryStartTimes || def.refs != 3
        || (char*)def.StoryStartTimes - (char*)&def != 0x44 || (char*)&def.StoryDuration - (char*)&def != 0x50) {
        std::printf("CTCVillage_00658a80_TEST FAIL\n");
        return 1;
    }
    std::printf("CTCVillage_00658a80_TEST PASS\n");
    return 0;
}
