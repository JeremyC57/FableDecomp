// 007153b0 behaviour test: CActionPickUpGenericBox::Update -- refresh an invalid box handle; once plugged in, finish
// when the creature is idle or runs another action id; a dead box finishes at once; an accepted pick-up action plugs
// in and records the creature's action id; a refused one puts the box into the carry slot from its carryable def
// (dropping whatever was held), adds carrying mode 34 and finishes. The temporary action and the def are released.
#include <cstdio>

static int g_liveActions = 0;
static int g_defRefs = 0;
struct CCarryableDef { long RefCount; long CarrySlot; };
struct CThing { CCarryableDef* def; bool alive; };
struct CDefPointer {
    CCarryableDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) { --Object->RefCount; --g_defRefs; } }
};
static bool GetSubDef(CThing* box, CDefPointer* out)
{
    if (!box->def) return false;
    out->Object = box->def; ++box->def->RefCount; ++g_defRefs; return true;
}
struct CTCCarrying {
    CThing* held; long heldSlot; int removes;
    CThing* RemoveThingFromPrimarySlot(bool) { ++removes; CThing* t = held; held = 0; return t; }
    void PutThingInCarrySlot(CThing* t, long slot, bool) { held = t; heldSlot = slot; }
};
struct CTCCreatureModeManager { unsigned long lastMode; int adds; bool AddMode(unsigned long m) { lastMode = m; ++adds; return true; } };
struct CAction {
    CThing* box; long id;
    CAction(long i, CThing* b) : box(b), id(i) { ++g_liveActions; }
    CAction(const CAction& o) : box(o.box), id(o.id) { ++g_liveActions; }
    ~CAction() { --g_liveActions; }
};
struct CCreature {
    bool busy; bool accepts; long currentID; long nextID;
    CTCCarrying carrying; CTCCreatureModeManager modes;
    bool IsPerformingAnyAction() const { return busy; }
    bool SetCurrentAction(const CAction& a) { if (!accepts) return false; currentID = a.id; busy = true; return true; }
};
struct CScriptThing {
    CThing* thing; bool valid; int refreshes;
    bool IsValid() const { return valid; }
    void Refresh() { ++refreshes; valid = true; }
    bool IsAlive() const { return thing && thing->alive; }
    CThing* GetPThing() const { return thing; }
};
struct CActionPickUpGenericBox {
    CCreature* creature; bool Terminated; CScriptThing PBox; long ActionID; bool ActionPluggedIn;
    void Update()
    {
        if (!PBox.IsValid()) PBox.Refresh();
        if (ActionPluggedIn) {
            if (!creature->IsPerformingAnyAction() || creature->currentID != ActionID) Terminated = true;
            return;
        }
        if (!PBox.IsAlive()) { Terminated = true; return; }
        if (creature->SetCurrentAction(CAction(creature->nextID, PBox.GetPThing()))) {
            ActionPluggedIn = true;
            ActionID = creature->currentID;
            return;
        }
        creature->carrying.RemoveThingFromPrimarySlot(false);
        CDefPointer def;
        GetSubDef(PBox.GetPThing(), &def);
        creature->carrying.PutThingInCarrySlot(PBox.GetPThing(), def.Object->CarrySlot, true);
        creature->modes.AddMode(0x22);
        Terminated = true;
    }
};

static int Fail(const char* why) { std::printf("007153b0_TEST FAIL (%s)\n", why); return 1; }

int main()
{
    CCarryableDef def = { 1, 3 };
    CThing box = { &def, true };
    CThing other = { 0, true };
    CCreature c; c.busy = false; c.accepts = true; c.currentID = 0; c.nextID = 41;
    c.carrying.held = &other; c.carrying.heldSlot = 0; c.carrying.removes = 0; c.modes.lastMode = 0; c.modes.adds = 0;
    CActionPickUpGenericBox a; a.creature = &c; a.Terminated = false; a.ActionID = 0; a.ActionPluggedIn = false;
    a.PBox.thing = &box; a.PBox.valid = false; a.PBox.refreshes = 0;
    a.Update();
    if (a.PBox.refreshes != 1 || !a.ActionPluggedIn || a.ActionID != 41 || a.Terminated) return Fail("accepted");
    if (g_liveActions != 0) return Fail("temporary released");
    a.Update();                                     // busy with action 41: keep going
    if (a.Terminated || a.PBox.refreshes != 1) return Fail("running");
    c.currentID = 7; a.Update();                    // another action: finished
    if (!a.Terminated) return Fail("other action");
    // refused: the box goes straight into the carry slot of its def, the carrying mode is added
    c.accepts = false; c.busy = false; a.Terminated = false; a.ActionPluggedIn = false;
    a.Update();
    if (!a.Terminated || c.carrying.removes != 1 || c.carrying.held != &box || c.carrying.heldSlot != 3) return Fail("refused carry");
    if (c.modes.adds != 1 || c.modes.lastMode != 0x22) return Fail("refused mode");
    if (g_defRefs != 0 || def.RefCount != 1 || g_liveActions != 0) return Fail("def released");
    // a dead box finishes at once without touching the creature
    box.alive = false; a.Terminated = false; a.Update();
    if (!a.Terminated || c.carrying.removes != 1) return Fail("dead box");
    std::printf("007153b0_TEST PASS\n");
    return 0;
}