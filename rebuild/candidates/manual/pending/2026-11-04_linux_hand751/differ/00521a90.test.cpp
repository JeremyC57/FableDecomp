// Behaviour test for CThingManager::SetNameForThing (0x00521A90, parked DIFFER row): a model of the row -- a new uid gets an empty script name, the display name is always stored.
#include <stdio.h>
#include <map>
#include <string>
static std::map<unsigned long long, std::pair<std::string, std::string> > g_names;
static void SetNameForThing(unsigned long long uid, const std::string& name) { if (g_names.find(uid) == g_names.end()) g_names[uid].first = ""; g_names[uid].second = name; }
static int Check() { g_names[5].first = "script"; SetNameForThing(5, "Bob"); SetNameForThing(7, "Ann");
    return g_names[5].first == "script" && g_names[5].second == "Bob" && g_names[7].first == "" && g_names[7].second == "Ann"; }
int main()
{
    int ok = Check();
    printf(ok ? "00521a90_TEST PASS\n" : "00521a90_TEST FAIL\n");
    return ok ? 0 : 1;
}
