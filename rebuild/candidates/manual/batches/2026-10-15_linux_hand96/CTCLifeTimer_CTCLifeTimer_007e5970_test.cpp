#include <cstdio>
struct CThing { bool m_force; int calls; void SetForceFrameUpdate(bool f) { m_force = f; ++calls; } };
struct CTCBase { CThing* m_pThing; long m_baseWord; CTCBase(CThing* o) : m_pThing(o), m_baseWord(0) { } virtual ~CTCBase() { } };
struct CTCLifeTimer : CTCBase {
    long FramesLeft; bool FadeOutBeforeKill; bool PerformOnDie;
    CTCLifeTimer(CThing* owner) : CTCBase(owner), FramesLeft(-1), FadeOutBeforeKill(false), PerformOnDie(true) { m_pThing->SetForceFrameUpdate(true); }
};
int main()
{
    CThing t; t.m_force = false; t.calls = 0;
    CTCLifeTimer* l = new CTCLifeTimer(&t);
    if (l->m_pThing != &t || l->FramesLeft != -1 || l->FadeOutBeforeKill || !l->PerformOnDie || !t.m_force || t.calls != 1) { std::printf("007e5970_TEST FAIL\n"); return 1; }
    delete l;
    std::printf("007e5970_TEST PASS\n");
    return 0;
}
