// Behaviour test for CWorldMap::~CWorldMap (0x00506940): close + release the WAD bank, remove maps 1..n-1, then free members in reverse order.
#include <cstdio>
#include <cstring>
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }
struct Info { int RefCount; };
struct Bank { void Close() { Log("close"); } };
struct Model {
    Bank* bank; Info* bankInfo;
    int mapCount;
    int regionCount;
    bool hasBrushes;
    void RemoveMap(int i) { char b[16]; std::sprintf(b, "rm%d", i); Log(b); }
    void Release(Info*& info, const char* what) { if (info) { if (--info->RefCount == 0) Log(what); } info = 0; }
    void Destroy()
    {
        if (bank) { bank->Close(); Release(bankInfo, "bankfree"); bank = 0; }
        for (int i = 1; i < mapCount; ++i) RemoveMap(i);
        for (int r = 0; r < regionCount; ++r) Log("region");
        if (hasBrushes) Log("erase");
        Log("base");
    }
};
int main()
{
    Bank b; Info inf; inf.RefCount = 1;
    Model m; m.bank = &b; m.bankInfo = &inf; m.mapCount = 3; m.regionCount = 2; m.hasBrushes = true;
    g_log[0] = 0; m.Destroy();
    if (std::strcmp(g_log, "close,bankfree,rm1,rm2,region,region,erase,base,") != 0) { std::printf("%s 00506940_TEST FAIL\n", g_log); return 1; }
    Info shared; shared.RefCount = 2;
    Model n; n.bank = &b; n.bankInfo = &shared; n.mapCount = 1; n.regionCount = 0; n.hasBrushes = false;
    g_log[0] = 0; n.Destroy();
    if (std::strcmp(g_log, "close,base,") != 0 || shared.RefCount != 1 || n.bankInfo != 0) { std::printf("%s 00506940_TEST FAIL\n", g_log); return 1; }
    Model k; k.bank = 0; k.bankInfo = 0; k.mapCount = 2; k.regionCount = 1; k.hasBrushes = false;
    g_log[0] = 0; k.Destroy();
    if (std::strcmp(g_log, "rm1,region,base,") != 0) { std::printf("%s 00506940_TEST FAIL\n", g_log); return 1; }
    std::printf("00506940_TEST PASS\n");
    return 0;
}
