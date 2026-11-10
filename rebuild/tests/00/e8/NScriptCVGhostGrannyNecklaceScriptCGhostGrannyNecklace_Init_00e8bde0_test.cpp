// Behaviour test for CGhostGrannyNecklace::Init (0x00E8BDE0): Init stores 1 in BrainState (+0x1C) and leaves the entity's other members alone.
#include <cstdio>
class CEntityScriptBase { public:
    void* Game; long Me[3];
    virtual ~CEntityScriptBase() { }
    virtual void Init() { }
};
struct CGhostGrannyNecklace : CEntityScriptBase {
    void* ParentClass; void* PMasterData; long BrainState;
    virtual void Init() { BrainState = 1; }
};
int main()
{
    CGhostGrannyNecklace e; e.Game = 0; e.ParentClass = &e; e.PMasterData = 0; e.BrainState = -7;
    CEntityScriptBase* base = &e;
    base->Init();
    bool ok = e.BrainState == 1 && e.ParentClass == &e && e.PMasterData == 0;
    if (!ok) { std::printf("00e8bde0_TEST FAIL\n"); return 1; }
    std::printf("00e8bde0_TEST PASS\n");
    return 0;
}