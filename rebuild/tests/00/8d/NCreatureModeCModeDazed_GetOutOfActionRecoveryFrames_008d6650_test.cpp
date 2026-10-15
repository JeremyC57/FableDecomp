// Behaviour test for ledger chunk 909 piece 008d6650
#include <cstdio>
static int g_baseTicks = 0, g_recovered = 0;
struct CThingCreatureBase { long GetOutOfActionRecoveryFrames() { return 42; } float GetStunnedTimeSeconds() { return 2.5f; } float GetDazedTimeSeconds() { return 1.5f; } };
struct CModeBase {
    virtual void v00() {} virtual void v01() {} virtual void v02() {}
    virtual void FrameUpdate() { ++g_baseTicks; }
    virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {} virtual void v12() {}
    CThingCreatureBase* ParentThing; unsigned char _body08[0x60];
};
namespace NCreatureMode {
#pragma pack(push, 1)
struct CModeOutOfActionBase : CModeBase {
    virtual long GetOutOfActionRecoveryFrames() { return 0; }
    virtual float GetOutOfActionTimeSeconds() = 0;
    virtual void Recover() = 0;
    long m_frames; bool m_canRecover; unsigned char _attacker[8];
    void FrameUpdate() { CModeBase::FrameUpdate(); if (m_frames > 0) --m_frames; if (m_canRecover && m_frames == 0) Recover(); }
};
#pragma pack(pop)
struct CModeDazed : CModeOutOfActionBase {
    virtual long GetOutOfActionRecoveryFrames() { return ParentThing->GetOutOfActionRecoveryFrames(); }
    virtual float GetOutOfActionTimeSeconds() { float seconds = ParentThing->GetDazedTimeSeconds(); return seconds; }
    virtual void Recover() { ++g_recovered; }
};
struct CModeStunned : CModeOutOfActionBase {
    virtual long GetOutOfActionRecoveryFrames() { return ParentThing->GetOutOfActionRecoveryFrames(); }
    virtual float GetOutOfActionTimeSeconds() { float seconds = ParentThing->GetStunnedTimeSeconds(); return seconds; }
    virtual void Recover() { ++g_recovered; }
};
}
int main()
{
    CThingCreatureBase creature; NCreatureMode::CModeDazed m; m.ParentThing = &creature;
    NCreatureMode::CModeOutOfActionBase& b = m; int ok = (b.GetOutOfActionRecoveryFrames() == 42);
    std::printf(ok ? "008d6650_TEST PASS\n" : "008d6650_TEST FAIL\n"); return ok ? 0 : 1;
}