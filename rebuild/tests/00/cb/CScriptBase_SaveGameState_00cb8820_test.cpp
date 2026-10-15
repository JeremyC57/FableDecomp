// Behaviour test for 0x00CB8820: CScriptBase::SaveGameState: save each entity script, then append script data and the entity scripts between their markers
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct Base {
    int saved[4]; int nsaved; unsigned long long ids[2]; int nids;
    void SaveEntityScript(unsigned long long id) { saved[nsaved++] = (int)id; }
    void PersistScriptData(char* out) { std::strcat(out, "D=1;\r\n"); }
    void SaveEntities(char* out) { std::strcat(out, "E;\r\n"); }
    void SaveGameState(char* out)
    {
        for (int i = 0; i < nids; ++i) SaveEntityScript(ids[i]);
        std::strcat(out, "START_SCRIPT_DATA\r\n");
        PersistScriptData(out);
        std::strcat(out, "END_SCRIPT_DATA\r\n\r\n");
        std::strcat(out, "START_ENTITY_SCRIPTS\r\n");
        SaveEntities(out);
        std::strcat(out, "END_ENTITY_SCRIPTS\r\n");
    }
};
static void Run()
{
    Base b; b.nsaved = 0; b.nids = 2; b.ids[0] = 4; b.ids[1] = 9;
    char out[256]; out[0] = 0; b.SaveGameState(out);
    CHECK(b.nsaved == 2 && b.saved[0] == 4 && b.saved[1] == 9);
    CHECK(std::strcmp(out, "START_SCRIPT_DATA\r\nD=1;\r\nEND_SCRIPT_DATA\r\n\r\nSTART_ENTITY_SCRIPTS\r\nE;\r\nEND_ENTITY_SCRIPTS\r\n") == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00cb8820_TEST FAIL\n"); return 1; }
    std::printf("00cb8820_TEST PASS\n");
    return 0;
}