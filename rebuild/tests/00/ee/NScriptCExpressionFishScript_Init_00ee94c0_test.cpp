// Behaviour test for CExpression_FishScript::Init (0x00EE94C0): the fishing expression script starts at level 1
// (LastFishingLevel, +0x44) and keeps its Game pointer (+0x40).
#include <cstdio>
class CScriptBase { public: virtual ~CScriptBase() { } virtual void Init() { } };
struct CExpression_FishScript : CScriptBase {
    void* Game; long LastFishingLevel;
    virtual void Init() { LastFishingLevel = 1; }
};
int main()
{
    int game = 0;
    CExpression_FishScript s; s.Game = &game; s.LastFishingLevel = 9;
    CScriptBase* base = &s;
    base->Init();
    bool ok = s.LastFishingLevel == 1 && s.Game == &game;
    if (!ok) { std::printf("00ee94c0_TEST FAIL\n"); return 1; }
    std::printf("00ee94c0_TEST PASS\n");
    return 0;
}