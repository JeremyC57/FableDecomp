// Behaviour test for NTCScriptedControl::CActionSummonerLightningAttackTarget::Update (0x007175D0): a fixture models the scripted
// control (parent creature), the embedded turn-to-face action, the script target (alive / thing), the creature's mode manager
// (present or absent, orb mode 90 active, current mode type) and its action slot (SetCurrentAction accepts or refuses, busy or idle),
// and runs the row's verbatim body through every branch, including multi-frame sequences.
#include <stdio.h>
static int failures;
static void check(bool ok, const char* what, int c) { if (!ok) { ++failures; printf("FAIL case %d: %s\n", c, what); } }
class CThing { public: int id; };
class CTCCreatureModeManager { public:
    class CModeBase { public: unsigned long type; virtual unsigned long GetType() const { return type; } };
    unsigned long active; CModeBase current; mutable int current_calls;
    bool IsModeActive(unsigned long mode) const { return active == mode; }
    CModeBase& GetCurrentMode() const { ++current_calls; return const_cast<CModeBase&>(current); } };
class CCreatureActionBase { public: virtual ~CCreatureActionBase() {} };
class CCreatureAction_SummonerLightningOrbInto;
class CThingCreatureBase : public CThing { public:
    CTCCreatureModeManager* modes; bool accept, busy; int set_calls, live_actions; CThing* last_target; CThingCreatureBase* last_creature;
    template<class T> bool PeekTC(T** out) const { if (!modes) return false; *out = modes; return true; }
    bool SetCurrentAction(const CCreatureActionBase& action);
    bool IsPerformingAnyAction() const { return busy; } };
static int live_orb_actions;
class CCreatureAction_SummonerLightningOrbInto : public CCreatureActionBase { public:
    CThingCreatureBase* creature; CThing* target;
    CCreatureAction_SummonerLightningOrbInto(CThingCreatureBase& c, CThing& t) : creature(&c), target(&t) { ++live_orb_actions; }
    ~CCreatureAction_SummonerLightningOrbInto() { --live_orb_actions; } };
bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase& action)
{
    const CCreatureAction_SummonerLightningOrbInto& orb = static_cast<const CCreatureAction_SummonerLightningOrbInto&>(action);
    ++set_calls; last_target = orb.target; last_creature = orb.creature; live_actions = live_orb_actions; return accept;
}
class CScriptThing { public: bool alive; CThing* thing; int alive_calls;
    virtual bool IsAlive() const { ++const_cast<CScriptThing*>(this)->alive_calls; return alive; }
    virtual CThing* GetPThing() const { return thing; } };
class CTCScriptedControl { public:
    CThingCreatureBase* ParentThing;
    CThingCreatureBase& GetParentCreature() { return *ParentThing; }
    class CActionBase { public:
        CTCScriptedControl* PTCScript; bool Activated, Terminated;
        CActionBase() : PTCScript(0), Activated(false), Terminated(false) {}
        virtual ~CActionBase() {}
        virtual void Update() {}
        bool IsTerminated() const { return Terminated; }
        void Terminate() { Terminated = true; }
        CThingCreatureBase& GetParentCreature() { return PTCScript->GetParentCreature(); } }; };
namespace NTCScriptedControl {
class CActionTurnToFacePosition : public CTCScriptedControl::CActionBase { public:
    int updates, finish_after;
    virtual void Update() { if (++updates >= finish_after) Terminate(); } };
class CActionSummonerLightningAttackTarget : public CTCScriptedControl::CActionBase { public:
    virtual void Update();
    CScriptThing PTarget; bool StartedAttack; bool TurnedToFace; CActionTurnToFacePosition TurnToFaceAction; };
void CActionSummonerLightningAttackTarget::Update()
{
    if (!TurnedToFace) {
        TurnToFaceAction.Update();
        if (!TurnToFaceAction.IsTerminated())
            return;
        TurnedToFace = true;
    }
    if (!StartedAttack) {
        CThingCreatureBase& parent_creature = GetParentCreature();
        const CTCCreatureModeManager* ptc_cmm;
        if (parent_creature.PeekTC(&ptc_cmm)) {
            if (ptc_cmm->IsModeActive(90)) {
                Terminate();
                return;
            }
        }
        if (!PTarget.IsAlive()) {
            Terminate();
            return;
        }
        if (parent_creature.SetCurrentAction(CCreatureAction_SummonerLightningOrbInto(parent_creature, *PTarget.GetPThing())))
            StartedAttack = true;
    }
    if (StartedAttack) {
        CThingCreatureBase& parent_creature = GetParentCreature();
        const CTCCreatureModeManager* ptc_cmm;
        parent_creature.PeekTC(&ptc_cmm);
        if (!parent_creature.IsPerformingAnyAction() && ptc_cmm->GetCurrentMode().GetType() != 90)
            Terminate();
    }
}
}
using NTCScriptedControl::CActionSummonerLightningAttackTarget;
struct World { CTCScriptedControl script; CThingCreatureBase creature; CTCCreatureModeManager modes; CThing target; CActionSummonerLightningAttackTarget a; };
static void setup(World& w, int turn_frames, bool has_modes, unsigned long active, bool alive, bool accept, bool busy, unsigned long current)
{
    w.creature.modes = has_modes ? &w.modes : 0; w.creature.accept = accept; w.creature.busy = busy; w.creature.set_calls = 0;
    w.creature.last_target = 0; w.creature.last_creature = 0; w.creature.live_actions = 0;
    w.modes.active = active; w.modes.current.type = current; w.modes.current_calls = 0; w.target.id = 7;
    w.script.ParentThing = &w.creature; w.a.PTCScript = &w.script; w.a.Terminated = false; w.a.StartedAttack = false; w.a.TurnedToFace = false;
    w.a.TurnToFaceAction.updates = 0; w.a.TurnToFaceAction.finish_after = turn_frames; w.a.TurnToFaceAction.Terminated = false;
    w.a.PTarget.alive = alive; w.a.PTarget.thing = &w.target; w.a.PTarget.alive_calls = 0;
}
int main()
{
    World w; int c = 0;
    // 1: still turning -> nothing else happens
    setup(w, 3, true, 0, true, true, true, 0); w.a.Update();
    check(!w.a.TurnedToFace && !w.a.Terminated && w.a.TurnToFaceAction.updates == 1 && w.creature.set_calls == 0 && w.a.PTarget.alive_calls == 0, "turning", ++c);
    // 2: turn done this frame, orb mode already active -> terminate, no action
    setup(w, 1, true, 90, true, true, false, 0); w.a.Update();
    check(w.a.TurnedToFace && w.a.Terminated && w.creature.set_calls == 0 && !w.a.StartedAttack && w.a.PTarget.alive_calls == 0, "orb mode active", ++c);
    // 3: target dead -> terminate
    setup(w, 1, true, 5, false, true, false, 0); w.a.Update();
    check(w.a.Terminated && w.creature.set_calls == 0 && w.a.PTarget.alive_calls == 1, "target dead", ++c);
    // 4: no mode manager, target dead -> terminate (mode check skipped)
    setup(w, 1, false, 90, false, true, false, 0); w.a.Update();
    check(w.a.Terminated && w.creature.set_calls == 0, "no modes, dead", ++c);
    // 5: attack refused -> not started, not terminated; temporary destroyed
    setup(w, 1, true, 0, true, false, false, 0); w.a.Update();
    check(!w.a.Terminated && !w.a.StartedAttack && w.creature.set_calls == 1 && w.creature.last_target == &w.target && w.creature.last_creature == &w.creature && w.creature.live_actions == 1 && live_orb_actions == 0 && w.modes.current_calls == 0, "refused", ++c);
    // 6: attack accepted, creature busy -> started, not terminated
    setup(w, 1, true, 0, true, true, true, 0); w.a.Update();
    check(w.a.StartedAttack && !w.a.Terminated && w.creature.set_calls == 1 && w.modes.current_calls == 0, "accepted busy", ++c);
    // 7: accepted, idle, current mode is the orb mode -> keep going
    setup(w, 1, true, 0, true, true, false, 90); w.a.Update();
    check(w.a.StartedAttack && !w.a.Terminated && w.modes.current_calls == 1, "idle in orb mode", ++c);
    // 8: accepted, idle, current mode other -> terminate the same frame
    setup(w, 1, true, 0, true, true, false, 3); w.a.Update();
    check(w.a.StartedAttack && w.a.Terminated && w.modes.current_calls == 1, "idle done", ++c);
    // 9: multi-frame: turn 2 frames, attack, busy, then idle in orb mode, then idle elsewhere
    setup(w, 2, true, 0, true, true, true, 90);
    w.a.Update(); check(!w.a.TurnedToFace && w.creature.set_calls == 0, "seq turn", ++c);
    w.a.Update(); check(w.a.TurnedToFace && w.a.StartedAttack && w.creature.set_calls == 1 && !w.a.Terminated, "seq start", ++c);
    w.a.Update(); check(w.creature.set_calls == 1 && !w.a.Terminated && w.a.TurnToFaceAction.updates == 2, "seq busy (no re-turn, no re-start)", ++c);
    w.creature.busy = false; w.a.Update(); check(!w.a.Terminated && w.modes.current_calls == 1, "seq orb", ++c);
    w.modes.current.type = 4; w.a.Update(); check(w.a.Terminated && w.creature.set_calls == 1, "seq end", ++c);
    // 10: refused then accepted on a later frame
    setup(w, 1, true, 0, true, false, false, 1);
    w.a.Update(); w.creature.accept = true; w.a.Update();
    check(w.creature.set_calls == 2 && w.a.StartedAttack && w.a.Terminated, "retry", ++c);
    if (failures) { printf("%d failures\n", failures); return 1; }
    puts("007175d0_TEST PASS");
    return 0;
}