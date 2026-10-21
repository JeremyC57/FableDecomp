// Behaviour test for CAT_BanditContact::Init (0x00F09640): two spawned callbacks registered in order, each over a
// counted copy of the contact's script thing, with "ParentClass." + callback name and an empty region name.
#include <cstdio>
#include <cstring>
struct Info { long refs; };
struct Added { char name[64]; const char* region; Info* info; };
static Added g_added[4];
static int g_count = 0;
static void Add(const char* prefix, const char* callback, Info* thingInfo)
{
    Added& a = g_added[g_count++];
    std::strcpy(a.name, prefix); std::strcat(a.name, callback);
    a.region = ""; a.info = thingInfo;
    if (thingInfo) ++thingInfo->refs;
}
static void Init(Info* thingInfo)
{
    Add("ParentClass.", "WatchForHeroAttackOnContact", thingInfo);
    Add("ParentClass.", "WatchForContactDeath", thingInfo);
}
int main()
{
    Info info = { 1 };
    Init(&info);
    bool ok = g_count == 2 && std::strcmp(g_added[0].name, "ParentClass.WatchForHeroAttackOnContact") == 0
        && std::strcmp(g_added[1].name, "ParentClass.WatchForContactDeath") == 0 && info.refs == 3
        && g_added[0].region[0] == 0 && g_added[1].info == &info;
    if (!ok) { std::printf("00f09640_TEST FAIL\n"); return 1; }
    std::printf("00f09640_TEST PASS\n");
    return 0;
}