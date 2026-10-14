#include <cstdio>
#include <math.h>
// Behaviour model of CAIStateGroup_HoverKeepInArea::IsRunnable @ 0x00959F30.
struct C3DVector { float X, Y, Z; };
struct CThingAICreature { C3DVector m_pos; C3DVector* GetPosition() { return &m_pos; } };
struct CAIBrain { unsigned char _pad[0x18]; CThingAICreature* PCreature; };
static bool g_baseRunnable = true; static int g_baseCalls = 0; static const float g_radius = 20.0f;
struct CAIStateGroupBase { virtual void v00() {} CAIBrain* PBrain; };
struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase { bool IsRunnable() { g_baseCalls++; return g_baseRunnable; } };
struct CAIStateGroup_HoverKeepInArea : CAIStateGroup_EnemyCreatureBase { C3DVector StartPos;
    bool IsRunnable() { C3DVector* p = PBrain->PCreature->GetPosition(); float dx = p->X - StartPos.X; float dy = p->Y - StartPos.Y;
        if ((float)sqrt(dy * dy + dx * dx) > g_radius) return CAIStateGroup_EnemyCreatureBase::IsRunnable(); return false; } };
int main()
{
    CThingAICreature c; c.m_pos.X = 12.0f; c.m_pos.Y = 16.0f; c.m_pos.Z = 100.0f; CAIBrain brain; brain.PCreature = &c;
    CAIStateGroup_HoverKeepInArea g; g.PBrain = &brain; g.StartPos.X = 0; g.StartPos.Y = 0; g.StartPos.Z = 0;
    bool ok = g.IsRunnable() == false && g_baseCalls == 0;           // distance 20 is not > 20
    c.m_pos.X = 13.0f; ok = ok && g.IsRunnable() == true && g_baseCalls == 1;
    g_baseRunnable = false; ok = ok && g.IsRunnable() == false && g_baseCalls == 2;
    if (!ok) { std::printf("00959f30_TEST FAIL\n"); return 1; }
    std::printf("00959f30_TEST PASS\n");
    return 0;
}
