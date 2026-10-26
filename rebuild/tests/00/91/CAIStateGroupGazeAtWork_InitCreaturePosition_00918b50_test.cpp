#include <cstdio>
// Behaviour model of CAIStateGroup_GazeAtWork::InitCreaturePosition @ 0x00918B50.
class C3DVector { public: float X, Y, Z; };
static C3DVector* g_placedAt = 0; static float g_radius = 0; static bool g_placeResult = true;
class CThingGameObject { public: C3DVector m_pos; C3DVector* GetPosition() { return &m_pos; } };
class CThingAICreature { public: bool AutoPlaceNear(C3DVector* p, float r) { g_placedAt = p; g_radius = r; return g_placeResult; } };
class CAIBrain { public: unsigned char _pad[0x18]; CThingAICreature* PCreature; };
static bool g_runnable = true; static CThingGameObject g_marker;
class CAIStateGroupBase { public: virtual bool IsRunnable() { return g_runnable; } CAIBrain* PBrain; };
struct CAIStateGroup_Gaze : CAIStateGroupBase { CThingGameObject* FindFreeGazePos() { return &g_marker; } };
struct CAIStateGroup_GazeAtWork : CAIStateGroup_Gaze {
    bool InitCreaturePosition() { if (IsRunnable()) { CThingGameObject* p = FindFreeGazePos(); CThingAICreature* c = PBrain->PCreature; return c->AutoPlaceNear(p->GetPosition(), 1.0f); } return false; } };
int main()
{
    CAIBrain brain; CThingAICreature creature; brain.PCreature = &creature;
    CAIStateGroup_GazeAtWork g; g.PBrain = &brain;
    bool ok = g.InitCreaturePosition() == true && g_placedAt == &g_marker.m_pos && g_radius == 1.0f;
    g_placeResult = false; ok = ok && g.InitCreaturePosition() == false;
    g_runnable = false; g_placedAt = 0; ok = ok && g.InitCreaturePosition() == false && g_placedAt == 0;
    if (!ok) { std::printf("00918b50_TEST FAIL\n"); return 1; }
    std::printf("00918b50_TEST PASS\n");
    return 0;
}