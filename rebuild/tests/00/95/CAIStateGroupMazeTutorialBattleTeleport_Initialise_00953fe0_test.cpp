#include <cstdio>
class CAIBrain { public:}; class CThingAICreature { public:}; class CCharString { public:};
class C3DVector { public: float X, Y, Z; C3DVector& operator=(const C3DVector& o) { X = o.X; Y = o.Y; Z = o.Z; return *this; } };
class CThingCreatureBase { public: C3DVector pos; const C3DVector& GetPosition() { return pos; } };
static long g_frame = 321; long GFGetCurrentFrame() { return g_frame; }
class CWorld { public: static long GetFrameCount() { return GFGetCurrentFrame(); } };
struct CAIBrainModel { unsigned char _pad[0x18]; CThingCreatureBase* Creature; };
static int g_base = 0;
class CAIStateGroupBase { public: virtual ~CAIStateGroupBase() {} CAIBrainModel* PBrain; unsigned char _pad[0xc]; CWorld w;
    void Initialise(CAIBrain&, CThingAICreature&, long, long, const CCharString&) { ++g_base; } CWorld* GetPWorld() { return &w; } };
struct CAIStateGroup_MazeTutorialBattleTeleport : CAIStateGroupBase { unsigned char _pad14[0xc]; long FrameOfLastTeleport; C3DVector LastTeleportPos;
    virtual void Initialise(CAIBrain&, CThingAICreature&, long, long, const CCharString&); };
void CAIStateGroup_MazeTutorialBattleTeleport::Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name)
{ CAIStateGroupBase::Initialise(brain, creature, id, priority, name); FrameOfLastTeleport = GetPWorld()->GetFrameCount(); LastTeleportPos = PBrain->Creature->GetPosition(); }
int main()
{
    CThingCreatureBase c; c.pos.X = 1; c.pos.Y = 2; c.pos.Z = 3; CAIBrainModel b; b.Creature = &c; CAIBrain br; CThingAICreature cr; CCharString n;
    CAIStateGroup_MazeTutorialBattleTeleport g; g.PBrain = &b; g.FrameOfLastTeleport = 0; g.LastTeleportPos.X = g.LastTeleportPos.Y = g.LastTeleportPos.Z = 0;
    g.Initialise(br, cr, 1, 2, n);
    if (g_base != 1 || g.FrameOfLastTeleport != 321 || g.LastTeleportPos.X != 1 || g.LastTeleportPos.Y != 2 || g.LastTeleportPos.Z != 3) { std::printf("00953fe0_TEST FAIL\n"); return 1; }
    std::printf("00953fe0_TEST PASS\n"); return 0;
}