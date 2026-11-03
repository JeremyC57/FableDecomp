#include <cstdio>
#include <cassert>
enum EGameflowPosition { EGP_INTRO = 0, EGP_TRAINING = 100, EGP_FINISHED_GAME = 2800 };
namespace NScript {
class CQ_SunnyvaleMasterData { public: EGameflowPosition PostSavePosition; int untouched; };
class CGameflowScript {
public:
    CQ_SunnyvaleMasterData& MasterScriptData;
    CGameflowScript(CQ_SunnyvaleMasterData& data):MasterScriptData(data){}
    unsigned long CoreQuestWaiting;
    int untouched;
    int declarations;
    void Init();
    void DeclareGossipCategories() {
        assert(MasterScriptData.PostSavePosition == EGP_INTRO);
        assert(CoreQuestWaiting == 0);
        ++declarations;
    }
};
}
void NScript::CGameflowScript::Init()
{
    MasterScriptData.PostSavePosition = EGP_INTRO;
    CoreQuestWaiting = 0;
    DeclareGossipCategories();
}
int main()
{
    for (int n = 0; n < 1024; ++n) {
        NScript::CQ_SunnyvaleMasterData a, b;
        a.PostSavePosition = (n & 1) ? EGP_TRAINING : EGP_FINISHED_GAME;
        a.untouched = n;
        b.PostSavePosition = EGP_TRAINING;
        b.untouched = ~n;
        NScript::CGameflowScript script(a);
        script.CoreQuestWaiting = 0xffffffffUL - n;
        script.untouched = n * 3;
        script.declarations = n;
        script.Init();
        assert(script.declarations == n + 1);
        assert(&script.MasterScriptData == &a && a.untouched == n);
        assert(script.untouched == n * 3);
        assert(b.PostSavePosition == EGP_TRAINING && b.untouched == ~n);
        script.Init();
        assert(script.declarations == n + 2);
    }
    std::puts("00ce6cf0_TEST PASS");
}
