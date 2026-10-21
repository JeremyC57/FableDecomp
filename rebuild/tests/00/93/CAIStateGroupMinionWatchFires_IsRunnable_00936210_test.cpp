// Behaviour test for CAIStateGroup_MinionWatchFires::IsRunnable (0x00936210): same logic over mocks.
#include <cstdio>
struct C3DVector { float x, y, z; };
struct CThing { bool dead; bool onFire; C3DVector pos; CThing() : dead(false), onFire(false) { pos.x = pos.y = pos.z = 0; } };
struct CTCIdleScheduler { bool preferred; bool IsPreferredIdle(void*) { return preferred; } };
struct CTCPerceiveThing { float radius; float GetSightRadius() { return radius; } };
struct CThingFilter_OnFire { CThingFilter_OnFire() { } bool operator()(const CThing* t) const { return t->onFire; } };
struct CThingFilter_In2DRange {
    C3DVector Pos; float Range;
    CThingFilter_In2DRange(const C3DVector& p, float r) : Pos(p), Range(r) { }
    bool operator()(const CThing* t) const { float dx = Pos.x - t->pos.x, dy = Pos.y - t->pos.y; return dx * dx + dy * dy < Range * Range; }
};
struct CThingCompare_Nearest {
    C3DVector m_pos; CThingCompare_Nearest(const C3DVector& p) : m_pos(p) { }
    float D(const CThing* t) const { float dx = m_pos.x - t->pos.x, dy = m_pos.y - t->pos.y, dz = m_pos.z - t->pos.z; return dx * dx + dy * dy + dz * dz; }
};
struct CWorldThings { CThing* things[4]; int n; };
static bool GetPBest(const CWorldThings& w, const CThingFilter_OnFire& a, const CThingFilter_In2DRange& b, const CThingCompare_Nearest& c, CThing** out)
{
    CThing* best = 0;
    for (int i = 0; i < w.n; ++i) {
        CThing* t = w.things[i];
        if (!t->dead && a(t) && b(t) && (best == 0 || c.D(t) < c.D(best))) best = t;
    }
    *out = best;
    return best != 0;
}
static bool IS_THING_ALIVE(CThing* p) { if (p != 0) { CThing* item = p; if (!item->dead) return true; } return false; }
struct CState {
    CThing* creature; CTCIdleScheduler* scheduler; CTCPerceiveThing* perceive; CWorldThings* world; CThing* PBuildingOnFire;
    bool IsRunnable()
    {
        if (scheduler && !scheduler->IsPreferredIdle(this)) return false;
        if (perceive) {
            CThing* building = 0;
            PBuildingOnFire = 0;
            float radius = perceive->GetSightRadius();
            CThingCompare_Nearest nearest(creature->pos);
            CThingFilter_In2DRange inRange(creature->pos, radius);
            if (GetPBest(*world, CThingFilter_OnFire(), inRange, nearest, &building)) {
                PBuildingOnFire = building;
                if (IS_THING_ALIVE(PBuildingOnFire)) return true;
            }
        }
        return false;
    }
};
int main()
{
    CThing creature, far1, near1, cold; far1.onFire = true; far1.pos.x = 8; near1.onFire = true; near1.pos.x = 3; cold.pos.x = 1;
    CWorldThings w; w.things[0] = &far1; w.things[1] = &near1; w.things[2] = &cold; w.n = 3;
    CTCIdleScheduler sch; sch.preferred = true; CTCPerceiveThing per; per.radius = 10.0f;
    CState s; s.creature = &creature; s.scheduler = &sch; s.perceive = &per; s.world = &w; s.PBuildingOnFire = 0;
    bool ok = s.IsRunnable() && s.PBuildingOnFire == &near1;
    per.radius = 2.0f; ok = ok && !s.IsRunnable() && s.PBuildingOnFire == 0;
    per.radius = 10.0f; sch.preferred = false; s.PBuildingOnFire = &cold; ok = ok && !s.IsRunnable() && s.PBuildingOnFire == &cold;
    s.scheduler = 0; s.perceive = 0; ok = ok && !s.IsRunnable();
    s.perceive = &per; ok = ok && s.IsRunnable();
    if (!ok) { std::printf("00936210_TEST FAIL\n"); return 1; }
    std::printf("00936210_TEST PASS\n");
    return 0;
}