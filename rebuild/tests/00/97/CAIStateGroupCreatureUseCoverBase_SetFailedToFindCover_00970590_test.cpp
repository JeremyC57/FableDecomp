#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CThingAICreature { public: C3DVector m_pos; const C3DVector& GetPosition() { return m_pos; } };
static long g_frame;
class CWorld { public: static long __fastcall GetFrame() { return g_frame; } };
class CAIBrain { public: unsigned char _pad[0x18]; CThingAICreature* Creature; };
static int g_worldCalls;
class CAIStateGroupBase { public: virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0xc]; CWorld* GetPWorld() { ++g_worldCalls; return 0; } };
struct CAIStateGroup_CreatureUseCoverBase : CAIStateGroupBase {
    unsigned char _pad2[0x20]; bool FailedToFindCover; long FailedTime; C3DVector FailedToFindCoverAt;
    void SetFailedToFindCover() { FailedToFindCover = true; FailedToFindCoverAt = PBrain->Creature->GetPosition(); FailedTime = GetPWorld()->GetFrame(); }
};
int main()
{
    CThingAICreature c; c.m_pos.X = 1; c.m_pos.Y = 2; c.m_pos.Z = 3; CAIBrain b; b.Creature = &c;
    CAIStateGroup_CreatureUseCoverBase g; g.PBrain = &b; g.FailedToFindCover = false; g_frame = 1234;
    g.SetFailedToFindCover();
    if (!g.FailedToFindCover || g.FailedTime != 1234 || g.FailedToFindCoverAt.X != 1 || g.FailedToFindCoverAt.Y != 2 || g.FailedToFindCoverAt.Z != 3 || g_worldCalls != 1) { std::printf("00970590_TEST FAIL\n"); return 1; }
    std::printf("00970590_TEST PASS\n"); return 0;
}