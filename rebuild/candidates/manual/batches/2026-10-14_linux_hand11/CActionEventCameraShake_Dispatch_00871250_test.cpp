#include <cstdio>

struct C3DVector { int v; };
static int g_created = 0; static long g_bone = -1, g_strength = -1; static unsigned char g_flag = 0; static C3DVector g_pos;
struct CThing { unsigned char _pad[0x91]; unsigned char m_isDead : 1;
    C3DVector* GetBonePosition(long bone, unsigned char flag) { g_bone = bone; g_flag = flag; return &g_pos; } };
struct CIntelligentPointer { virtual void v00() { } CThing* PItem; CThing* GetPItem() { return PItem; } };
struct CActionEventContext { unsigned char _pad[8]; CIntelligentPointer m_pThing; };
void __fastcall CreateCameraShake(C3DVector* position, long strength) { if (position == &g_pos) ++g_created; g_strength = strength; }
struct CActionEventCameraShake { virtual void v00() { } unsigned char _pad[0x10]; long m_strength; long m_bone; unsigned char m_boneFlag;
    void Dispatch(const CActionEventContext& context)
    {
        CThing* thing = const_cast<CIntelligentPointer&>(context.m_pThing).GetPItem();
        if (thing && !thing->m_isDead)
            CreateCameraShake(thing->GetBonePosition(m_bone, m_boneFlag), m_strength);
    } };
int main()
{
    CThing t; t.m_isDead = 0; CActionEventContext ctx; ctx.m_pThing.PItem = &t;
    CActionEventCameraShake e; e.m_strength = 3; e.m_bone = 7; e.m_boneFlag = 1;
    e.Dispatch(ctx); if (g_created != 1 || g_bone != 7 || g_flag != 1 || g_strength != 3) { std::printf("00871250_TEST FAIL\n"); return 1; }
    t.m_isDead = 1; e.Dispatch(ctx); if (g_created != 1) { std::printf("00871250_TEST FAIL\n"); return 1; }
    ctx.m_pThing.PItem = 0; e.Dispatch(ctx); if (g_created != 1) { std::printf("00871250_TEST FAIL\n"); return 1; }
    std::printf("00871250_TEST PASS\n"); return 0;
}
