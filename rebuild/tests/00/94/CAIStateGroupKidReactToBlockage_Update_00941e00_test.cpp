// Behaviour test for CAIStateGroup_KidReactToBlockage::Update (0x00941E00): model copy with scripted stubs.
#include <cstdio>
#include <cstring>

struct CCharString { const char* s; CCharString(const char* t, long = -1) : s(t) { } };
struct C3DVector { float X, Y, Z; };
struct CThing { unsigned char m_flags91; C3DVector pos; C3DVector* GetPosition() { return &pos; } };
struct CThingCreatureBase { int id; };
struct CPointer { CThing* item; CThing* GetPItem() const { return item; } };
struct CSoundPair { long FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
struct CCreatureActionBase { const char* anim; bool useMove, usePhysics; CThingCreatureBase* owner; };
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    CCreatureAction_PlayAnimation(CThingCreatureBase* o, const CCharString& n, const CSoundPair& snd, bool m, bool p, bool, long, long)
    { anim = n.s; useMove = m; usePhysics = p; owner = o; if (snd.FirstSound || snd.LastSound) anim = 0; }
};
struct CAIBrain { CThingCreatureBase* PCreature; CThingCreatureBase* GetPCreature() const { return PCreature; } };

static int g_look, g_lookResult, g_terminated, g_played, g_results[8];
static const char* g_anims[8];
static float g_lookX, g_lookSpeed;

struct CAIStateGroup_KidReactToBlockage {
    CAIBrain* PBrain;
    CPointer PThingCreature;
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed)
    { ++g_look; g_lookX = pos.X; g_lookSpeed = speed; return (repeat == 1 && flags == 1) ? g_lookResult : -1; }
    long AIAction_PerformActionAndWait(CCreatureActionBase* a, bool wait)
    {
        if (wait || !a->usePhysics || a->useMove || a->owner != PBrain->PCreature) return -1;
        g_anims[g_played] = a->anim; return g_results[g_played++];
    }
    void Terminate() { ++g_terminated; }
    void Update();
};

#define IS_THING_ALIVE(p) ((p) != 0 && !((p)->m_flags91 & 1))

void CAIStateGroup_KidReactToBlockage::Update()
{
    if (IS_THING_ALIVE(PThingCreature.GetPItem())) {
        CThing* thing = PThingCreature.GetPItem();
        if (AIAction_LookAtPos(1, *thing->GetPosition(), 1, 0.013888889f) == 1) {
            Terminate();
            return;
        }
    }
    if (IS_THING_ALIVE(PThingCreature.GetPItem())) {
        bool failed;
        {
            CCharString anim("ST_TANTRUM_INTO");
            CCreatureAction_PlayAnimation action(PBrain->GetPCreature(), anim, CSoundPair(), false, true, false, 0, 0);
            failed = AIAction_PerformActionAndWait(&action, false) != 2;
        }
        if (!failed) {
            {
                CCharString anim("ST_TANTRUM_LOOP");
                CCreatureAction_PlayAnimation action(PBrain->GetPCreature(), anim, CSoundPair(), false, true, false, 0, 0);
                failed = AIAction_PerformActionAndWait(&action, false) != 2;
            }
            if (!failed) {
                CCharString anim("ST_TANTRUM_OUTOF");
                CCreatureAction_PlayAnimation action(PBrain->GetPCreature(), anim, CSoundPair(), false, true, false, 0, 0);
                failed = AIAction_PerformActionAndWait(&action, false) != 2;
            }
        }
        if (failed) {
            Terminate();
            return;
        }
    }
    Terminate();
}

static void Reset() { g_look = g_terminated = g_played = 0; g_lookResult = 2; for (int i = 0; i < 8; ++i) { g_results[i] = 2; g_anims[i] = 0; } }

int main()
{
    int ok = 1;
    CThing kid = { 0, { 4.0f, 0, 0 } };
    CThingCreatureBase me = { 1 };
    CAIBrain brain = { &me };
    CAIStateGroup_KidReactToBlockage s; s.PBrain = &brain; s.PThingCreature.item = &kid;
    // full tantrum
    Reset(); s.Update();
    if (g_look != 1 || g_lookX != 4.0f || g_lookSpeed != 0.013888889f || g_played != 3 || g_terminated != 1) ok = 0;
    if (!g_anims[2] || std::strcmp(g_anims[0], "ST_TANTRUM_INTO") || std::strcmp(g_anims[1], "ST_TANTRUM_LOOP") || std::strcmp(g_anims[2], "ST_TANTRUM_OUTOF")) ok = 0;
    // look-at returns 1: terminate without a tantrum
    Reset(); g_lookResult = 1; s.Update();
    if (g_played != 0 || g_terminated != 1) ok = 0;
    // the loop animation fails: out-of is skipped
    Reset(); g_results[1] = 3; s.Update();
    if (g_played != 2 || g_terminated != 1) ok = 0;
    // the into animation fails
    Reset(); g_results[0] = 0; s.Update();
    if (g_played != 1 || g_terminated != 1) ok = 0;
    // dead creature: no look, no tantrum
    Reset(); kid.m_flags91 = 1; s.Update();
    if (g_look != 0 || g_played != 0 || g_terminated != 1) ok = 0;
    // no creature
    Reset(); s.PThingCreature.item = 0; s.Update();
    if (g_look != 0 || g_played != 0 || g_terminated != 1) ok = 0;
    if (!ok) { std::printf("00941e00_TEST FAIL\n"); return 1; }
    std::printf("00941e00_TEST PASS\n");
    return 0;
}