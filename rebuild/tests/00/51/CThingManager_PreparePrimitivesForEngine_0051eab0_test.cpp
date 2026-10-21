// Behaviour test for CThingManager::PreparePrimitivesForEngine (0x0051EAB0): the latch copies FirstRenderOfFrame,
// bullet-time things get the rescaled copy, dead / unrendered things leave the render list.
#include <cstdio>

struct CInfo { float Current; float Last; int Tag; };
struct CBulletTime {
    bool m_active; void* m_pController;
    float Convert(float f) { return f * 0.5f; }
    bool IsActive() const { return m_active || m_pController != 0; }
};
struct CThing {
    CThing* m_pNextRender; bool Dead; bool Rendered; bool RenderedAfter; bool Affected;
    int Calls; float SeenCurrent; bool Removed;
    bool IsRenderedThisFrame() { return Calls ? RenderedAfter : Rendered; }
    bool IsAffectedByBulletTime() { return Affected; }
    void PreparePrimitivesForEngine(const CInfo& info) { ++Calls; SeenCurrent = info.Current; }
};
static bool g_Latch;
struct CThingManager {
    CBulletTime* BT; bool FirstRenderOfFrame; CThing* Head; bool UpdateAndRenderAllFlag;
    CBulletTime* GetBulletTime() { return BT; }
    void RemoveFromRenderList(CThing* t) { t->Removed = true; }
    void PreparePrimitivesForEngine(const CInfo& info)
    {
        g_Latch = FirstRenderOfFrame;
        CInfo bulletTimeInfo = info;
        bool bulletTime;
        if (!GetBulletTime()->IsActive()) {
            bulletTime = false;
        } else {
            bulletTime = true;
            bulletTimeInfo.Current = GetBulletTime()->Convert(bulletTimeInfo.Current);
            bulletTimeInfo.Last = GetBulletTime()->Convert(bulletTimeInfo.Last);
        }
        CThing* thing = Head;
        while (thing) {
            CThing* next = thing->m_pNextRender;
            if (!thing->Dead && (thing->IsRenderedThisFrame() || UpdateAndRenderAllFlag)) {
                if (bulletTime && thing->IsAffectedByBulletTime())
                    thing->PreparePrimitivesForEngine(bulletTimeInfo);
                else
                    thing->PreparePrimitivesForEngine(info);
                if (!thing->IsRenderedThisFrame() && !UpdateAndRenderAllFlag)
                    RemoveFromRenderList(thing);
            } else {
                RemoveFromRenderList(thing);
            }
            thing = next;
        }
        FirstRenderOfFrame = false;
    }
};

static void Init(CThing& t, CThing* next, bool dead, bool r, bool ra, bool aff)
{
    t.m_pNextRender = next; t.Dead = dead; t.Rendered = r; t.RenderedAfter = ra; t.Affected = aff;
    t.Calls = 0; t.SeenCurrent = 0; t.Removed = false;
}

int main()
{
    CThing a, b, c, d;
    Init(a, &b, false, true, true, true);
    Init(b, &c, true, true, true, true);
    Init(c, &d, false, false, false, false);
    Init(d, 0, false, true, false, false);
    CBulletTime bt; bt.m_active = true; bt.m_pController = 0;
    CThingManager m; m.BT = &bt; m.FirstRenderOfFrame = true; m.Head = &a; m.UpdateAndRenderAllFlag = false;
    CInfo info; info.Current = 2.0f; info.Last = 4.0f; info.Tag = 1;
    m.PreparePrimitivesForEngine(info);
    bool ok = g_Latch && !m.FirstRenderOfFrame;
    ok = ok && a.Calls == 1 && a.SeenCurrent == 1.0f && !a.Removed;
    ok = ok && b.Calls == 0 && b.Removed;
    ok = ok && c.Calls == 0 && c.Removed;
    ok = ok && d.Calls == 1 && d.SeenCurrent == 2.0f && d.Removed;
    if (!ok) { std::printf("0051eab0_TEST FAIL\n"); return 1; }
    std::printf("0051eab0_TEST PASS\n");
    return 0;
}