// Behaviour test for 00893c00 CGameScriptInterface::MsgOnHeroHairTypeChanged: area -> message type, def name copied out.
#include <cstdio>
#include <cstring>
struct Message { long type; const char* defName; };
static const Message* Find(const Message* all, int n, long type) { const Message* r = 0; for (int i = 0; i < n; ++i) if (all[i].type == type) r = &all[i]; return r; }
static bool MsgOnHeroHairTypeChanged(const Message* all, int n, long area, char* name)
{
    long type = area == 0x10 ? 0x39 : area == 0x80 ? 0x3a : 0x3b;
    const Message* m = Find(all, n, type);
    if (m) { std::strcpy(name, m->defName); return true; }
    return false;
}
int main()
{
    Message m[3] = { { 0x39, "HAIR_A" }, { 0x3a, "BEARD_B" }, { 0x39, "HAIR_C" } };
    char name[32] = "";
    bool a = MsgOnHeroHairTypeChanged(m, 3, 0x10, name);
    bool okA = a && !std::strcmp(name, "HAIR_C");
    bool b = MsgOnHeroHairTypeChanged(m, 3, 0x80, name);
    bool okB = b && !std::strcmp(name, "BEARD_B");
    bool c = MsgOnHeroHairTypeChanged(m, 3, 0x01, name);
    if (!okA || !okB || c || std::strcmp(name, "BEARD_B")) { std::printf("00893c00_TEST FAIL\n"); return 1; }
    std::printf("00893c00_TEST PASS\n");
    return 0;
}