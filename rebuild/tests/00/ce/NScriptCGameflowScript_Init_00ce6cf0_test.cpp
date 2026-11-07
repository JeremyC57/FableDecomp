// Behaviour model of NScript::CGameflowScript::Init (0x00CE6CF0): PostSavePosition = EGP_INTRO, CoreQuestWaiting = 0, then the gossip
// categories are declared (once).
#include <cstdio>
enum { EGP_NONE = -1, EGP_INTRO = 0 };
struct MasterData { long PostSavePosition; };
struct Gameflow {
    MasterData* MasterScriptData; unsigned long CoreQuestWaiting; int Declared;
    void DeclareGossipCategories() { ++Declared; }
    void Init() { MasterScriptData->PostSavePosition = EGP_INTRO; CoreQuestWaiting = 0; DeclareGossipCategories(); }
};
int main()
{
    MasterData m = { 7 };
    Gameflow g = { &m, 5, 0 };
    g.Init();
    if (m.PostSavePosition != EGP_INTRO || g.CoreQuestWaiting != 0 || g.Declared != 1) { std::printf("FAIL\n"); return 1; }
    std::printf("00ce6cf0_TEST PASS\n");
    return 0;
}