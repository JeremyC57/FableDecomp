// 0096af60 behaviour test: CAIStateGroup_WhisperPotionCombat::DisappearJump (control flow of the final.cpp body)
#ifndef _MSC_VER
#define __forceinline inline
#endif
#include <stdio.h>
#include <string.h>

static char g_log[512];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }

class C3DVector { public: float x, y, z; };
struct CThingGraphic {
    const C3DVector* pos; long flags;
    void SetPosition(const C3DVector* p) { pos = p; Log("pos"); }
    void SetFlags(long f) { flags = f; Log("flags"); }
    void Restore() { Log("restore"); }
};
class CThing { public:
    CThingGraphic* PGraphic;
    unsigned char m_flags91;
    void SetActive(bool a) { Log(a ? "active1" : "active0"); }
};
class CThingCreatureBase;
struct CIntelligentPointerThing {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
class CCreatureActionBase { public: int kind; };
class CCreatureAction_WhisperJumpIntoSky : public CCreatureActionBase { public:
    CCreatureAction_WhisperJumpIntoSky(CThingCreatureBase*, CThing*) { kind = 1; }
};
struct CCreatureAction_WhisperLand : CCreatureActionBase {
    CCreatureAction_WhisperLand(CThingCreatureBase*) { kind = 2; }
};
struct CThingCreature : CThing { };
class CAIBrain { public: CThingCreature* PCreature; };

static long g_frame;
static long g_frameStep;
class CWorld { public: static long __fastcall GetFrame() { long f = g_frame; g_frame += g_frameStep; return f; } };
long g_FramesPerSecond = 10;

static int g_stopAt;      // IsStopping() returns true from this call number on (0 = never)
static int g_stopCalls;
static long g_jumpResults[8];
static int g_jumpCalls;
static int g_nothingCalls;
static int g_terminated;
static CWorld g_world;

class CAIStateGroupBase { public:
    CAIBrain* PBrain;
    CWorld* GetPWorld() const { return &g_world; }
    bool IsStopping() { ++g_stopCalls; return g_stopAt != 0 && g_stopCalls >= g_stopAt; }
    void Terminate() { ++g_terminated; Log("terminate"); }
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool)
    {
        if (action->kind == 2) { Log("land"); return 2; }
        Log("jump");
        return g_jumpResults[g_jumpCalls++];
    }
    long AIAction_DoNothing() { ++g_nothingCalls; return 0; }
};

struct CAIStateGroup_WhisperPotionCombat : CAIStateGroupBase {
    CIntelligentPointerThing PTarget;
    C3DVector Jump_to_pos;
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PBrain->PCreature; }
    void DisappearJump();
    bool JumpIntoSky();
};

__forceinline bool CAIStateGroup_WhisperPotionCombat::JumpIntoSky()
{
    long result;
    do {
        if (PTarget.GetPItem() == 0 || (PTarget.GetPItem()->m_flags91 & 1))
            return false;
        CCreatureAction_WhisperJumpIntoSky action(GetCreature(), PTarget.GetPItem());
        result = AIAction_PerformActionAndWait(&action, false);
    } while (!IsStopping() && result != 2);
    if (IsStopping())
        return false;
    return true;
}

void CAIStateGroup_WhisperPotionCombat::DisappearJump()
{
    if (!JumpIntoSky()) {
        Terminate();
        return;
    }
    PBrain->PCreature->PGraphic->SetPosition(&Jump_to_pos);
    PBrain->PCreature->SetActive(false);
    PBrain->PCreature->PGraphic->SetFlags(0x20);
    long start = GetPWorld()->GetFrame();
    long now;
    do {
        AIAction_DoNothing();
        now = GetPWorld()->GetFrame();
    } while (!IsStopping() && (float)(now - start) < (float)g_FramesPerSecond * 0.5f);
    PBrain->PCreature->PGraphic->Restore();
    PBrain->PCreature->SetActive(true);
    CCreatureAction_WhisperLand land(GetCreature());
    AIAction_PerformActionAndWait(&land, false);
}

static int g_fail;
static void Run(const char* name, CThing* target, unsigned char flags, int stopAt, long r0, long r1, long r2,
    long step, const char* expectLog, int expectNothing)
{
    CThingGraphic graphic; graphic.pos = 0; graphic.flags = 0;
    CThingCreature creature; creature.PGraphic = &graphic; creature.m_flags91 = 0;
    CAIBrain brain; brain.PCreature = &creature;
    CAIStateGroup_WhisperPotionCombat g;
    g.PBrain = &brain;
    if (target) target->m_flags91 = flags;
    g.PTarget.m_pItem = target;
    g_log[0] = 0; g_stopAt = stopAt; g_stopCalls = 0; g_jumpCalls = 0; g_nothingCalls = 0; g_terminated = 0;
    g_jumpResults[0] = r0; g_jumpResults[1] = r1; g_jumpResults[2] = r2;
    g_frame = 100; g_frameStep = step;
    g.DisappearJump();
    if (strcmp(g_log, expectLog) != 0 || g_nothingCalls != expectNothing
        || (strstr(expectLog, "pos") && (graphic.pos != &g.Jump_to_pos || graphic.flags != 0x20))) {
        printf("FAIL %s: log '%s' nothing %d\n", name, g_log, g_nothingCalls);
        g_fail = 1;
    }
}

int main()
{
    CThing target; target.PGraphic = 0; target.m_flags91 = 0;
    // no target -> terminate before any jump
    Run("null target", 0, 0, 0, 2, 2, 2, 1, "terminate;", 0);
    // dead target -> terminate
    Run("dead target", &target, 1, 0, 2, 2, 2, 1, "terminate;", 0);
    // first jump lands (result 2), never stopping: hide for 5 frames (half of 10 fps), restore, land
    Run("happy", &target, 0, 0, 2, 0, 0, 1, "jump;pos;active0;flags;restore;active1;land;", 5);
    // jump retried until result 2
    Run("retry", &target, 0, 0, 0, 1, 2, 1, "jump;jump;jump;pos;active0;flags;restore;active1;land;", 5);
    // stopping during the jump loop -> terminate (loop test + post-loop test)
    Run("stop in jump", &target, 0, 1, 0, 0, 0, 1, "jump;terminate;", 0);
    // stopping during the hide loop -> restore early and land
    Run("stop in hide", &target, 0, 4, 2, 0, 0, 1, "jump;pos;active0;flags;restore;active1;land;", 2);
    // big frame steps end the hide after one wait
    Run("fast frames", &target, 0, 0, 2, 0, 0, 10, "jump;pos;active0;flags;restore;active1;land;", 1);
    // target dies between retries -> terminate after the first jump
    {
        CThing t2; t2.PGraphic = 0; t2.m_flags91 = 0;
        Run("retry then null", 0, 0, 0, 0, 0, 0, 1, "terminate;", 0);
    }
    if (g_fail) return 1;
    printf("0096af60_TEST PASS\n");
    return 0;
}