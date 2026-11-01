// behaviour test: NScript::CQ_SecretPassageScript::~CQ_SecretPassageScript (implicit) -- members are destroyed in
// reverse declaration order (seh_Mother, minions[10..0], FlagMap, GlobalMymap), then the CLevelScriptBase /
// CScriptBase base; the plain bool/long members need nothing.
#include <cstdio>
static char g_log[64]; static int g_n = 0;
struct Tracked { char tag; explicit Tracked(char t = 'm') : tag(t) { } ~Tracked() { if (g_n < 63) g_log[g_n++] = tag; } };
class CScriptBase { public: virtual ~CScriptBase() { g_log[g_n++] = 'B'; } };
struct CLevelScriptBase : CScriptBase { };
struct CQ_SecretPassageScript : CLevelScriptBase {
    bool MissionFailed; long GhostSightings;
    Tracked GlobalMymap; Tracked FlagMap; Tracked minions[11]; Tracked seh_Mother;
    CQ_SecretPassageScript() : GlobalMymap('G'), FlagMap('F'), seh_Mother('S') { }
};
int main()
{
    CScriptBase* p = new CQ_SecretPassageScript;
    delete p;
    g_log[g_n] = 0;
    const char* want = "SmmmmmmmmmmmFGB";
    for (int i = 0; want[i] || g_log[i]; ++i)
        if (want[i] != g_log[i]) { std::printf("00df10d0_TEST FAIL\n"); return 1; }
    std::printf("00df10d0_TEST PASS\n");
    return 0;
}