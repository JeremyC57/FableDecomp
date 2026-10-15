#include <cstdio>
struct CScriptThing { unsigned char m_body[0xc]; };
struct CGameCalls {
    int calls; CScriptThing* t516; long a516; CScriptThing* t517; long a517, b517;
    virtual void Slot516(CScriptThing* thing, long a) { ++calls; t516 = thing; a516 = a; }
    virtual void Slot517(CScriptThing* thing, long a, long b) { ++calls; t517 = thing; a517 = a; b517 = b; }
};
struct CSpectator {
    long m_0; CGameCalls* PGame; CScriptThing Thing; void* ParentClass; void* PMasterData; bool FacingThunder;
    void Init() { FacingThunder = true; PGame->Slot516(&Thing, 0); PGame->Slot517(&Thing, 0, 1); }
};
int main()
{
    CGameCalls g; g.calls = 0; g.a516 = 9; g.a517 = 9; g.b517 = 9;
    CSpectator s; s.PGame = &g; s.FacingThunder = false;
    s.Init();
    bool ok = s.FacingThunder && g.calls == 2 && g.t516 == &s.Thing && g.a516 == 0 && g.t517 == &s.Thing && g.a517 == 0 && g.b517 == 1;
    if (!ok) { std::printf("00e99890_TEST FAIL\n"); return 1; }
    std::printf("00e99890_TEST PASS\n");
    return 0;
}
