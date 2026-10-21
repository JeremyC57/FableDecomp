// Behaviour test for CTCVillage::OnHeroSlept (0x0065935E): live villagers with the 0xC3 interface get its timer
// cleared; villagers whose 0x1F interface is present but inactive get a brain reset, and those of them needing
// auto-placement are re-placed; villagers without it or with it active are untouched; the crier is reset once.
#include <cstdio>
#include <list>

struct CTCBase { bool active; float Timer; bool IsActive() const { return active; } };
struct CThingAICreature {
    bool alive, hasTimer, hasMode, autoPlace;
    CTCBase timerTC, modeTC;
    int resets, placements;
    bool HasTC(long t) const { return t == 0xc3 ? hasTimer : t == 0x1f ? hasMode : false; }
    template <class T> T* GetTC(long t) { return t == 0xc3 ? &timerTC : &modeTC; }
    void ResetBrain() { ++resets; }
    void InitCreaturePosition() { ++placements; }
    bool RequiresAutoPlacement() { return autoPlace; }
};
struct CPtr { CThingAICreature* p; CThingAICreature* GetPItem() const { return p; } };
static bool IS_THING_ALIVE(const CPtr& t) { return t.p && t.p->alive; }

struct CTCVillage {
    std::list<CPtr> VillagePeople;
    int crierResets;
    void ResetCrier() { ++crierResets; }
    void OnHeroSlept();
};

void CTCVillage::OnHeroSlept()
{
    std::list<CPtr>::iterator node;
    for (node = VillagePeople.begin(); node != VillagePeople.end(); ++node) {
        if (IS_THING_ALIVE(*node)) {
            CThingAICreature* person = node->GetPItem();
            if (person->HasTC(0xc3))
                person->GetTC<CTCBase>(0xc3)->Timer = 0.0f;
        }
    }
    for (node = VillagePeople.begin(); node != VillagePeople.end(); ++node) {
        CThingAICreature* person = node->GetPItem();
        if (person->HasTC(0x1f) && !person->GetTC<CTCBase>(0x1f)->IsActive())
            node->GetPItem()->ResetBrain();
    }
    for (node = VillagePeople.begin(); node != VillagePeople.end(); ++node) {
        CThingAICreature* person = node->GetPItem();
        if (person->HasTC(0x1f) && !person->GetTC<CTCBase>(0x1f)->IsActive()
            && node->GetPItem()->RequiresAutoPlacement())
            node->GetPItem()->InitCreaturePosition();
    }
    ResetCrier();
}

static CThingAICreature Make(bool alive, bool hasTimer, bool hasMode, bool active, bool autoPlace)
{
    CThingAICreature c;
    c.alive = alive; c.hasTimer = hasTimer; c.hasMode = hasMode; c.autoPlace = autoPlace;
    c.timerTC.active = true; c.timerTC.Timer = 9.0f; c.modeTC.active = active; c.modeTC.Timer = 0;
    c.resets = 0; c.placements = 0;
    return c;
}

int main()
{
    CThingAICreature a = Make(true, true, true, false, true);    // timer cleared, reset, placed
    CThingAICreature b = Make(false, true, true, false, false);  // dead: timer kept; reset, not placed
    CThingAICreature c = Make(true, false, true, true, true);    // active: untouched
    CThingAICreature d = Make(true, true, false, false, true);   // no 0x1F: only the timer
    CTCVillage v; v.crierResets = 0;
    CPtr pa = { &a }, pb = { &b }, pc = { &c }, pd = { &d };
    v.VillagePeople.push_back(pa); v.VillagePeople.push_back(pb); v.VillagePeople.push_back(pc); v.VillagePeople.push_back(pd);
    v.OnHeroSlept();
    int ok = a.timerTC.Timer == 0.0f && a.resets == 1 && a.placements == 1
        && b.timerTC.Timer == 9.0f && b.resets == 1 && b.placements == 0
        && c.resets == 0 && c.placements == 0
        && d.timerTC.Timer == 0.0f && d.resets == 0 && d.placements == 0
        && v.crierResets == 1;
    std::printf(ok ? "0065935e_TEST PASS\n" : "0065935e_TEST FAIL\n");
    return ok ? 0 : 1;
}