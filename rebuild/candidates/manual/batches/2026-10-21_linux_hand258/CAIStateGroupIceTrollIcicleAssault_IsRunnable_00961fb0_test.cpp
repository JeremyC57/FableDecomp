// Behaviour test for CAIStateGroup_IceTrollIcicleAssault::IsRunnable (0x00961FB0): model copy with stub callees.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct CThing { unsigned char m_flags91; C3DVector pos; const C3DVector& GetPosition() const { return pos; } };
struct CPointer { CThing* m_pItem; CPointer& operator=(CThing* t) { m_pItem = t; return *this; } };
struct CSnowTrollDef { long RefCount; float IceTrailDistanceLimit; };
static int g_releases;
struct CDefPointer {
    CSnowTrollDef* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p) { --m_p->RefCount; ++g_releases; } }
    CSnowTrollDef* operator->() const { return m_p; }
};
static CSnowTrollDef* g_def;
struct CDefClass { bool GetSubDef(CDefPointer* out) const { out->m_p = g_def; if (g_def) ++g_def->RefCount; return g_def != 0; } };
struct CTCTargetingAI { CThing* target; CThing* GetPTarget() { return target; } };
struct CTCBoss { bool ready; bool IsReadyForPhaseChange() const { return ready; } };
struct CCreature { CThing thing; CTCTargetingAI* targeting; CTCBoss* boss; CDefClass defClass; };
struct CAIBrain { CCreature* PCreature; };

struct CAIStateGroup_IceTrollIcicleAssault {
    CAIBrain* PBrain; CPointer PTarget;
    bool IsRunnable();
};

bool CAIStateGroup_IceTrollIcicleAssault::IsRunnable()
{
    CTCTargetingAI* targeting = PBrain->PCreature->targeting;
    if (targeting != 0) {
        CThing* target = targeting->GetPTarget();
        if (target != 0 && !(target->m_flags91 & 1)) {
            CCreature* creature = PBrain->PCreature;
            if (creature->boss != 0 && creature->boss->IsReadyForPhaseChange()) {
                PTarget = target;
                return true;
            }
            CDefPointer def;
            bool hasLimit = PBrain->PCreature->defClass.GetSubDef(&def) && def->IceTrailDistanceLimit > 0.0f;
            if (hasLimit) {
                const C3DVector& me = PBrain->PCreature->thing.GetPosition();
                const C3DVector& him = target->GetPosition();
                float dx = him.X - me.X;
                float dy = him.Y - me.Y;
                float limit = def->IceTrailDistanceLimit;
                if (dx * dx + dy * dy > limit * limit) {
                    PTarget = target;
                    return true;
                }
            }
        }
    }
    return false;
}

int main()
{
    int ok = 1;
    CThing target = { 0, { 0, 12, 50 } };
    CTCTargetingAI targeting = { &target };
    CTCBoss boss = { false };
    CSnowTrollDef def = { 1, 10.0f };
    CCreature c = { { 0, { 0, 0, 0 } }, &targeting, &boss, CDefClass() };
    CAIBrain brain = { &c };
    CAIStateGroup_IceTrollIcicleAssault s; s.PBrain = &brain; s.PTarget.m_pItem = 0;
    g_def = &def;
    // 12 units away, limit 10: runnable, target cached, def reference released
    if (!s.IsRunnable() || s.PTarget.m_pItem != &target || def.RefCount != 1 || g_releases != 1) ok = 0;
    // inside the limit
    s.PTarget.m_pItem = 0; target.pos.Y = 9;
    if (s.IsRunnable() || s.PTarget.m_pItem != 0 || def.RefCount != 1) ok = 0;
    // boss ready: runnable regardless of distance
    boss.ready = true; if (!s.IsRunnable() || s.PTarget.m_pItem != &target) ok = 0; boss.ready = false;
    // no limit (0) or no def
    def.IceTrailDistanceLimit = 0; target.pos.Y = 100; if (s.IsRunnable()) ok = 0;
    g_def = 0; if (s.IsRunnable()) ok = 0; g_def = &def; def.IceTrailDistanceLimit = 10.0f;
    // dead target, no target, no targeting AI
    target.m_flags91 = 1; if (s.IsRunnable()) ok = 0; target.m_flags91 = 0;
    targeting.target = 0; if (s.IsRunnable()) ok = 0; targeting.target = &target;
    c.targeting = 0; if (s.IsRunnable()) ok = 0; c.targeting = &targeting;
    if (!s.IsRunnable() || def.RefCount != 1) ok = 0;
    if (!ok) { std::printf("00961fb0_TEST FAIL\n"); return 1; }
    std::printf("00961fb0_TEST PASS\n");
    return 0;
}
