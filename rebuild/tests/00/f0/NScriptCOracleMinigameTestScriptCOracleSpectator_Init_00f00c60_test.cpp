// Behaviour test for COracleSpectator::Init (0x00F00C60): Init stores 10 in NextLine (+0x1C) and leaves the entity's other members alone.
#include <cstdio>
class CEntityScriptBase { public:
    void* Game; long Me[3];
    virtual ~CEntityScriptBase() { }
    virtual void Init() { }
};
struct COracleSpectator : CEntityScriptBase {
    void* ParentClass; void* PMasterData; long NextLine;
    virtual void Init() { NextLine = 10; }
};
int main()
{
    COracleSpectator e; e.Game = 0; e.ParentClass = &e; e.PMasterData = 0; e.NextLine = -7;
    CEntityScriptBase* base = &e;
    base->Init();
    bool ok = e.NextLine == 10 && e.ParentClass == &e && e.PMasterData == 0;
    if (!ok) { std::printf("00f00c60_TEST FAIL\n"); return 1; }
    std::printf("00f00c60_TEST PASS\n");
    return 0;
}