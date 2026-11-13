#include <cstdio>
#include <cstring>
// Model of SetRelevantRegion: data string -> region + village def name looked up nearest the hero.
enum { REGION_BOWERSTONE=1,REGION_OAKVALE=2,REGION_BANDITCAMP=3,REGION_KNOTHOLE=4 };
struct Parent { int RelevantRegion; const char* Village; };
struct Ring {
    const char* data; bool terminating; Parent* ParentClass; int lookups;
    const char* GetNearestWithDefName(const char* def) { ++lookups; return def; }
    bool IsActiveThreadTerminating() const { return terminating; }
    void SetRelevantRegion()
    {
        if (std::strcmp(data, "BowerstoneSlums") == 0) {
            if (IsActiveThreadTerminating()) return;
            ParentClass->RelevantRegion = REGION_BOWERSTONE;
            do { ParentClass->Village = GetNearestWithDefName("VILLAGE_BOWERSTONE_SLUMS"); } while (0);
        } else if (std::strcmp(data, "KnotholeGlade") == 0) {
            if (IsActiveThreadTerminating()) return;
            ParentClass->RelevantRegion = REGION_KNOTHOLE;
            ParentClass->Village = GetNearestWithDefName("VILLAGE_KNOTHOLE_GLADE");
        } else if (std::strcmp(data, "Oakvale") == 0) {
            if (IsActiveThreadTerminating()) return;
            ParentClass->RelevantRegion = REGION_OAKVALE;
            ParentClass->Village = GetNearestWithDefName("VILLAGE_OAKVALE");
        } else if (std::strcmp(data, "BanditCamp") == 0) {
            if (IsActiveThreadTerminating()) return;
            ParentClass->RelevantRegion = REGION_BANDITCAMP;
            ParentClass->Village = GetNearestWithDefName("VILLAGE_BANDIT_CAMP_RESIDENTIAL");
        } else {
            if (IsActiveThreadTerminating()) return;
            ParentClass->RelevantRegion = REGION_KNOTHOLE;
        }
    }
};
static bool check(const char* d, bool term, int region, const char* village)
{
    Parent p; p.RelevantRegion = 6; p.Village = 0;
    Ring r; r.data = d; r.terminating = term; r.ParentClass = &p; r.lookups = 0;
    r.SetRelevantRegion();
    if (p.RelevantRegion != region) return false;
    if (village == 0) return p.Village == 0 && r.lookups == 0;
    return p.Village && std::strcmp(p.Village, village) == 0 && r.lookups == 1;
}
int main()
{
    bool ok = check("BowerstoneSlums", false, 1, "VILLAGE_BOWERSTONE_SLUMS")
           && check("KnotholeGlade", false, 4, "VILLAGE_KNOTHOLE_GLADE")
           && check("Oakvale", false, 2, "VILLAGE_OAKVALE")
           && check("BanditCamp", false, 3, "VILLAGE_BANDIT_CAMP_RESIDENTIAL")
           && check("Somewhere", false, 4, 0)
           && check("", false, 4, 0)
           && check("Oakvale", true, 6, 0)
           && check("Elsewhere", true, 6, 0);
    if (!ok) { std::printf("00e83230_TEST FAIL\n"); return 1; }
    std::printf("00e83230_TEST PASS\n");
    return 0;
}