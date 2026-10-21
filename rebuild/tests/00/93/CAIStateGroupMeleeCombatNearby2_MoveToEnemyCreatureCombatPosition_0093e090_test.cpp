// Behaviour test for CAIStateGroup_MeleeCombatNearby2::MoveToEnemyCreatureCombatPosition (0x0093E090): model copy, scripted stubs.
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

struct C3DVector { float X, Y, Z; };
struct CThingCreatureBase {
    u64 UID;
    int combatant;
    bool IsRegisteredCombatant() { return combatant != 0; }
};
static long g_frame;
static int g_prefCalls, g_unreachable;
struct CCombatManager {
    C3DVector GetPreferredPos(CThingCreatureBase*) { C3DVector v; v.X = (float)(++g_prefCalls); v.Y = 0; v.Z = 0; return v; }
    void OnCombatPositionUnreachable(CThingCreatureBase*) { ++g_unreachable; }
};
struct CWorld {
    CCombatManager* CombatManager;
    CCombatManager* GetPCombatManager() const { return CombatManager; }
    static long GetWorldFrame() { return g_frame; }
};
struct CAIBrain { CThingCreatureBase* PCreature; };
struct CPointer { CThingCreatureBase* item; CThingCreatureBase* GetPItem() const { return item; } };

static int g_runnable[16], g_inPos[16], g_move[16];
static int g_ir, g_ip, g_im;
static float g_lastPosX;

struct CAIStateGroup_MeleeCombatNearby2 {
    CAIBrain* PBrain; CWorld* world_; CPointer PEnemy;
    CWorld* GetPWorld() { return world_; }
    bool IsStillRunnable(bool) { return g_runnable[g_ir++] != 0; }
    bool IsEnemyCreatureInCombatPosition() { return g_inPos[g_ip++] != 0; }
    long AIAction_MoveToPosProximity(long, C3DVector pos, float radius, bool* arrived, long, bool, bool, bool, CThingCreatureBase*, bool, float)
    {
        g_lastPosX = pos.X; if (radius != 0.5f) return -1; *arrived = false; ++g_frame; return g_move[g_im++];
    }
    bool MoveToEnemyCreatureCombatPosition();
};

bool CAIStateGroup_MeleeCombatNearby2::MoveToEnemyCreatureCombatPosition()
{
    bool arrived = false;
    CThingCreatureBase* creature = PBrain->PCreature;
    CWorld* world = GetPWorld();
    C3DVector pos = world->GetPCombatManager()->GetPreferredPos(creature);
    long lastFrame = CWorld::GetWorldFrame();
    if (IsStillRunnable(true)) {
        do {
            if (IsEnemyCreatureInCombatPosition())
                return true;
            if (!PBrain->PCreature->IsRegisteredCombatant())
                return false;
            CThingCreatureBase* me = PBrain->PCreature;
            u64 uid = me->UID;
            if ((uid + CWorld::GetWorldFrame()) % 5 == 0 || CWorld::GetWorldFrame() - lastFrame > 6) {
                CThingCreatureBase* self = PBrain->PCreature;
                CWorld* w = GetPWorld();
                pos = w->GetPCombatManager()->GetPreferredPos(self);
                lastFrame = CWorld::GetWorldFrame();
            }
            long result = AIAction_MoveToPosProximity(2, pos, 0.5f, &arrived, 5, true, false, true, PEnemy.GetPItem(), false, 0.0f);
            if (result == 3) {
                if (PBrain->PCreature->IsRegisteredCombatant()) {
                    CThingCreatureBase* self = PBrain->PCreature;
                    CWorld* w = GetPWorld();
                    w->CombatManager->OnCombatPositionUnreachable(self);
                }
            } else if (result != 4 && result != 2)
                return false;
        } while (IsStillRunnable(true));
    }
    return true;
}

static void Reset() { g_ir = g_ip = g_im = 0; g_prefCalls = 0; g_unreachable = 0; for (int i = 0; i < 16; ++i) { g_runnable[i] = 1; g_inPos[i] = 0; g_move[i] = 4; } }

int main()
{
    int ok = 1;
    CThingCreatureBase c = { 3, 1 };
    CCombatManager cm; CWorld w = { &cm }; CAIBrain b = { &c };
    CAIStateGroup_MeleeCombatNearby2 s; s.PBrain = &b; s.world_ = &w; s.PEnemy.item = &c;
    // not runnable at once: true without moving
    Reset(); g_runnable[0] = 0; g_frame = 0;
    if (!s.MoveToEnemyCreatureCombatPosition() || g_im != 0 || g_prefCalls != 1) ok = 0;
    // frame 1: uid 3 + 1 = 4 (no refresh); frame 2: 3+2 = 5 -> refresh; then in position
    Reset(); g_frame = 1; g_inPos[2] = 1;
    if (!s.MoveToEnemyCreatureCombatPosition()) ok = 0;
    if (g_im != 2 || g_prefCalls != 2 || g_lastPosX != 2.0f) ok = 0;
    // a move returning 3 tells the combat manager, failure (1) returns false
    Reset(); g_frame = 1; g_move[0] = 3; g_move[1] = 1;
    if (s.MoveToEnemyCreatureCombatPosition() || g_unreachable != 1 || g_im != 2) ok = 0;
    // stale position (more than 6 frames) refreshes: uid 1 + frame 10 = 11, 11 % 5 != 0, but 10 - 0 > 6
    Reset(); c.UID = 1; g_frame = 0; g_inPos[1] = 1;
    {
        // starts at frame 0; the first iteration at frame 0: 1 % 5 != 0 and 0 - 0 <= 6: no refresh
        if (!s.MoveToEnemyCreatureCombatPosition() || g_prefCalls != 1) ok = 0;
    }
    Reset(); g_frame = 10; c.UID = 1; g_inPos[1] = 1;
    if (!s.MoveToEnemyCreatureCombatPosition() || g_prefCalls != 1) ok = 0;   // lastFrame taken at entry: no refresh
    // creature drops out of combat: false
    Reset(); c.combatant = 0;
    if (s.MoveToEnemyCreatureCombatPosition()) ok = 0;
    if (!ok) { std::printf("0093e090_TEST FAIL\n"); return 1; }
    std::printf("0093e090_TEST PASS\n");
    return 0;
}