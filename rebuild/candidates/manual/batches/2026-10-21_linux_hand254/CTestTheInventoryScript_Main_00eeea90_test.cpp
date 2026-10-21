// Behaviour test for CTestTheInventoryScript::Main (0x00EEEA90): the flag is cleared, the script yields until the
// level "Empty" is loaded, spawns a "TestForLampOn" thread bound to this script (region ""), yields twice and
// activates "QuestNotInScripts"; a terminating thread stops at the first check.
#include <cstdio>
#include <cstring>

struct CCharString {
    char text[64];
    CCharString(const char* t, long = -1) { std::strncpy(text, t, 63); text[63] = 0; }
};
static int g_frame = 0, g_loadedAt = 0, g_terminateAt = 1000;
static int g_quests = 0, g_lampOn = 0;
static char g_quest[64];

struct CSpawnedFuncBase {
    char name[64];
    unsigned long id;
    CSpawnedFuncBase(const CCharString& n, unsigned long i) : id(i) { std::strcpy(name, n.text); }
    virtual ~CSpawnedFuncBase() {}
    virtual void SuspendableProcess() = 0;
};
template <class T> struct CSpawnedFunc0 : CSpawnedFuncBase {
    void (T::*Func)();
    T* PClass;
    CSpawnedFunc0(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frame; }
    bool MsgIsLevelLoaded(const CCharString& level) const { return std::strcmp(level.text, "Empty") == 0 && g_frame >= g_loadedAt; }
    void ActivateQuest(const CCharString& quest) const { ++g_quests; std::strcpy(g_quest, quest.text); }
};
static CSpawnedFuncBase* g_spawned = 0;
static char g_region[64];
struct CScriptBase {
    bool IsActiveThreadTerminating() { return g_frame >= g_terminateAt; }
    void AddSpawnedFunction(CSpawnedFuncBase* f, const CCharString& region) { g_spawned = f; std::strcpy(g_region, region.text); }
};
struct CTestTheInventoryScript : CScriptBase {
    CGameScriptInterface* GSI;
    bool TestFlag;
    void Main();
    void TestForLampOn() { ++g_lampOn; }
};

void CTestTheInventoryScript::Main()
{
    TestFlag = false;
    while (!GSI->MsgIsLevelLoaded("Empty")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddSpawnedFunction(new CSpawnedFunc0<CTestTheInventoryScript>("TestForLampOn", 0, this,
                                                                 &CTestTheInventoryScript::TestForLampOn),
                       CCharString("", -1));
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    GSI->ActivateQuest("QuestNotInScripts");
}

int main()
{
    CGameScriptInterface gsi;
    CTestTheInventoryScript s; s.GSI = &gsi; s.TestFlag = true;
    g_loadedAt = 3;
    s.Main();
    int ok = !s.TestFlag && g_frame == 5 && g_quests == 1 && std::strcmp(g_quest, "QuestNotInScripts") == 0
             && g_spawned && std::strcmp(g_spawned->name, "TestForLampOn") == 0 && g_spawned->id == 0
             && g_region[0] == 0;
    if (ok) { g_spawned->SuspendableProcess(); ok = g_lampOn == 1; }
    delete g_spawned; g_spawned = 0;
    g_frame = 0; g_loadedAt = 10; g_terminateAt = 2; g_quests = 0;
    s.Main();
    ok = ok && g_frame == 2 && g_spawned == 0 && g_quests == 0;
    if (!ok) { std::printf("00eeea90_TEST FAIL\n"); return 1; }
    std::printf("00eeea90_TEST PASS\n");
    return 0;
}
