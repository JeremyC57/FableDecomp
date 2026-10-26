#include <cstdio>
#include <cstring>
// behaviour test: StopFollowingThing terminates a running follow action, otherwise queues an EXPRESSION_WAIT event at the target
class CCharString { public: const char* t; CCharString(const char* s, long) : t(s) {} };
struct CThing_Retail;
static const char* g_terminated = 0; static int g_events = 0; static CThing_Retail* g_eventTarget = 0; static long g_refs = 0;
class CTCScriptedControl { public: long CurrentActionType; bool TerminateActionByName(const CCharString& n) { g_terminated = n.t; return true; } };
class CTCEntityEvents { public: void AddEvent(void*) { ++g_events; } };
struct CThing_Retail {
    bool m_dead; CTCScriptedControl* control; CTCEntityEvents* events;
    bool GetInterface(CTCScriptedControl** out, long) { *out = control; return true; }
    bool GetInterface(CTCEntityEvents** out, long) { if (!events) return false; *out = events; return true; }
};
class CScriptThing { public: CThing_Retail* t; bool alive; bool IsAlive() const { return alive; } CThing_Retail* GetPThing() const { return t; } };
class CExpressionDef { public: long RefCount; };
template<class T> class CDefPointer { public:
    T* Object; CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object = 0; } }
};
static CExpressionDef g_wait;
class CGameDefinitionManager { public:
    static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
    bool GetDef(const CCharString& name, CDefPointer<CExpressionDef>* out)
    { if (std::strcmp(name.t, "EXPRESSION_WAIT")) return false; out->Object = &g_wait; ++g_wait.RefCount; return true; }
};
namespace NEntityEvents {
class CEventExpressionPerformedTo { public:
    CEventExpressionPerformedTo(void* target, long, const CDefPointer<CExpressionDef>& def, long, long)
    { g_eventTarget = (CThing_Retail*)target; g_refs = def.Object ? def.Object->RefCount : -1; }
};
}
static const char g_actionFollowCreature[] = "CActionFollowCreature";
static const char g_expressionWait[] = "EXPRESSION_WAIT";
struct CIntelligentPointer_Retail { CThing_Retail* item; CThing_Retail* GetPItem() { return item; } };
struct CScriptGameResourceObjectScriptedThing_Methods {
    CIntelligentPointer_Retail PThingPtr;
    void StopFollowingThing(const CScriptThing& target);
};
void CScriptGameResourceObjectScriptedThing_Methods::StopFollowingThing(const CScriptThing& target)
{
    CIntelligentPointer_Retail* thing = &PThingPtr;
    if (thing->GetPItem()) {
        if (!thing->GetPItem()->m_dead) {
            if (target.IsAlive()) {
                CTCScriptedControl* control;
                thing->GetPItem()->GetInterface(&control, 0x1f);
                if (control->CurrentActionType == 4) {
                    CCharString name(g_actionFollowCreature, -1);
                    control->TerminateActionByName(name);
                } else {
                    CThing_Retail* targetThing = target.GetPThing();
                    CThing_Retail* me = thing->GetPItem();
                    CDefPointer<CExpressionDef> def;
                    {
                        CCharString name(g_expressionWait, -1);
                        CGameDefinitionManager::Get().GetDef(name, &def);
                    }
                    CTCEntityEvents* events;
                    if (me->GetInterface(&events, 0x42)) {
                        new NEntityEvents::CEventExpressionPerformedTo(targetThing, 0, def, -1, -1);
                        events->AddEvent(0);
                    }
                }
            }
        }
    }
}
int main()
{
    CTCScriptedControl control; control.CurrentActionType = 4; CTCEntityEvents events;
    CThing_Retail me = { false, &control, &events }, other = { false, 0, 0 };
    CScriptGameResourceObjectScriptedThing_Methods s; s.PThingPtr.item = &me;
    CScriptThing target = { &other, true };
    g_wait.RefCount = 1;
    s.StopFollowingThing(target);
    bool ok = g_terminated && std::strcmp(g_terminated, "CActionFollowCreature") == 0 && g_events == 0;
    control.CurrentActionType = 1;
    s.StopFollowingThing(target);
    ok = ok && g_events == 1 && g_eventTarget == &other && g_refs == 2 && g_wait.RefCount == 1;
    target.alive = false; s.StopFollowingThing(target); ok = ok && g_events == 1;
    if (!ok) { std::printf("009041a0_TEST FAIL\n"); return 1; }
    std::printf("009041a0_TEST PASS\n");
    return 0;
}