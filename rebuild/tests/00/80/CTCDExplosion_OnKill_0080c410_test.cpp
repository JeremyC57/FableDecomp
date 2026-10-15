// Test for 0080c410 CTCDExplosion::OnKill: every emitter thing that is set and alive is killed; dead/null ones skipped.
#include <cstdio>
struct CThing { bool Dead; int KillCalls; void Kill(bool c) { if (c) ++KillCalls; } };
struct CBaseIntelligentPointer { CThing* PItem; CThing* GetPItem() const { return PItem; } };
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->Dead)
            return true;
    }
    return false;
}
struct CEmitterEntry { CBaseIntelligentPointer first; long second; };
struct CEmitterVector {
    CEmitterEntry* _M_start; CEmitterEntry* _M_finish;
    CEmitterEntry* begin() { return _M_start; }
    CEmitterEntry* end() { return _M_finish; }
};
struct CTCDExplosion {
    CEmitterVector Emitters;
    virtual ~CTCDExplosion() {}
    virtual void OnKill()
    {
        for (CEmitterEntry* it = Emitters.begin(); it != Emitters.end(); ++it) {
            if (IS_THING_ALIVE(it->first))
                it->first.GetPItem()->Kill(true);
        }
    }
};
int main()
{
    CThing a = { false, 0 }, b = { true, 0 }, c = { false, 0 };
    CEmitterEntry e[4] = { { { &a }, 1 }, { { &b }, 2 }, { { 0 }, 3 }, { { &c }, 4 } };
    CTCDExplosion x; x.Emitters._M_start = e; x.Emitters._M_finish = e;
    x.OnKill();                                         // empty vector
    if (a.KillCalls != 0) { std::printf("0080c410_TEST FAIL\n"); return 1; }
    x.Emitters._M_finish = e + 4;
    CTCDExplosion* p = &x; p->OnKill();
    if (a.KillCalls != 1 || b.KillCalls != 0 || c.KillCalls != 1) { std::printf("0080c410_TEST FAIL\n"); return 1; }
    std::printf("0080c410_TEST PASS\n");
    return 0;
}