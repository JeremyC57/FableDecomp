// Behaviour test for 0x00F25E70 CArenaCreatureDef::Transfer: four keyed transfers in member order, the long defaults 0,
// the CCharString defaults from GetDefaultVal (an empty string).
#include <cstdio>
#include <cstring>

class CCharString { public: const char* Text; };
struct Rec { const char* name; void* value; int kind; long defLong; const char* defText; };
static Rec g_rec[8];
static int g_n = 0;

class CPersistContext { public:
    static CCharString GetDefaultVal() { CCharString s; s.Text = ""; return s; }
    void Transfer(const char* name, CCharString& value, const CCharString& def)
    { Rec r = { name, &value, 1, 0, def.Text }; g_rec[g_n++] = r; }
    void Transfer(const char* name, long& value, const long& def)
    { Rec r = { name, &value, 2, def, 0 }; g_rec[g_n++] = r; }
    void Transfer(const char* name, CCharString& value) { Transfer(name, value, GetDefaultVal()); }
};

class CArenaCreatureDef { public:
    CCharString CreatureType;
    long        NumCreatures;
    CCharString HUDType;
    long        DeathScore;
    void Transfer(CPersistContext& context)
    {
        context.Transfer("CreatureType", CreatureType);
        context.Transfer("NumCreatures", NumCreatures, 0L);
        context.Transfer("HUDType", HUDType);
        context.Transfer("DeathScore", DeathScore, 0L);
    }
};

int main()
{
    CArenaCreatureDef d;
    CPersistContext c;
    d.Transfer(c);
    const char* names[4] = { "CreatureType", "NumCreatures", "HUDType", "DeathScore" };
    void* vals[4] = { &d.CreatureType, &d.NumCreatures, &d.HUDType, &d.DeathScore };
    int kinds[4] = { 1, 2, 1, 2 };
    if (g_n != 4) { std::printf("00f25e70_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i) {
        if (std::strcmp(g_rec[i].name, names[i]) != 0 || g_rec[i].value != vals[i] || g_rec[i].kind != kinds[i]) {
            std::printf("00f25e70_TEST FAIL\n"); return 1;
        }
        if (kinds[i] == 2 && g_rec[i].defLong != 0) { std::printf("00f25e70_TEST FAIL\n"); return 1; }
        if (kinds[i] == 1 && std::strcmp(g_rec[i].defText, "") != 0) { std::printf("00f25e70_TEST FAIL\n"); return 1; }
    }
    std::printf("00f25e70_TEST PASS\n");
    return 0;
}