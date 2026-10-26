#include <cstdio>
// behaviour test: WaitForEntityToFinishPerformingTasks queues one wait action (targeting the entity's thing) when the
// scripted thing is alive and the entity is alive; the by-value script-thing temporaries are destroyed
static int g_live = 0, g_added = 0;
struct CThing_Retail;
class CScriptThing { public:
    CThing_Retail* t;
    CScriptThing(CThing_Retail* x) : t(x) { ++g_live; }
    CScriptThing(const CScriptThing& o) : t(o.t) { ++g_live; }
    ~CScriptThing() { --g_live; }
    bool IsAlive() const;
    CThing_Retail* GetPThing() const { return t; }
};
struct CThing_Retail { bool m_dead; CScriptThing GetScriptThing() const { return CScriptThing((CThing_Retail*)this); } };
bool CScriptThing::IsAlive() const { return t && !t->m_dead; }
struct CActionBase { virtual ~CActionBase() {} };
static CThing_Retail* g_target = 0;
struct CActionWaitForThingToFinishPerformingTasks : CActionBase {
    CScriptThing Target;
    CActionWaitForThingToFinishPerformingTasks(CThing_Retail* target) : Target(target->GetScriptThing()) { g_target = Target.t; }
};
class CTCScriptedControl { public: void AddAction(CActionBase* a) { ++g_added; delete a; } };
static CTCScriptedControl g_control;
struct CThingWithControl : CThing_Retail { bool GetInterface(CTCScriptedControl** out, long) { *out = &g_control; return true; } };
struct CIntelligentPointer_Retail { CThingWithControl* item; CThingWithControl* GetPItem() { return item; } };
struct CEntity { CThing_Retail* thing; CScriptThing GetScriptThing() const { return CScriptThing(thing); } };
struct CScriptGameResourceObjectScriptedThing_Methods {
    CIntelligentPointer_Retail PThing;
    void WaitForEntityToFinishPerformingTasks(const CEntity& entity);
};
void CScriptGameResourceObjectScriptedThing_Methods::WaitForEntityToFinishPerformingTasks(const CEntity& entity)
{
    CIntelligentPointer_Retail* thing = &PThing;
    if (thing->GetPItem() && !thing->GetPItem()->m_dead && entity.GetScriptThing().IsAlive()) {
        CTCScriptedControl* control;
        thing->GetPItem()->GetInterface(&control, 0x1f);
        control->AddAction(new CActionWaitForThingToFinishPerformingTasks(entity.GetScriptThing().GetPThing()));
    }
}
int main()
{
    CThingWithControl me; me.m_dead = false;
    CThing_Retail other; other.m_dead = false;
    CScriptGameResourceObjectScriptedThing_Methods s; s.PThing.item = &me;
    CEntity e; e.thing = &other;
    s.WaitForEntityToFinishPerformingTasks(e);
    bool ok = g_added == 1 && g_target == &other && g_live == 0;
    other.m_dead = true; s.WaitForEntityToFinishPerformingTasks(e); ok = ok && g_added == 1 && g_live == 0;
    other.m_dead = false; me.m_dead = true; s.WaitForEntityToFinishPerformingTasks(e); ok = ok && g_added == 1 && g_live == 0;
    s.PThing.item = 0; s.WaitForEntityToFinishPerformingTasks(e); ok = ok && g_added == 1;
    if (!ok) { std::printf("00904540_TEST FAIL\n"); return 1; }
    std::printf("00904540_TEST PASS\n");
    return 0;
}