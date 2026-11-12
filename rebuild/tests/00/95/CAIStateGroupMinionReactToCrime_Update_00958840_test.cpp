// Behaviour test for CAIStateGroup_MinionReactToCrime::Update (0x00958840): a copy of the model's control flow over small fakes --
// home building force-open bump, duplicate guard, perceived-thing gate, def lookup + message post, opinion flag set/clear, hero
// follower quit, FindAGuardAndYellForHelp retry while 4, GuardTold on 2, def released, Terminate always.
#include <stdio.h>
struct Def { long refs; int deleted; void Delete() { deleted = 1; } };
struct DefPtr { Def* Object; DefPtr() : Object(0) {} ~DefPtr() { if (Object) { Def* o = Object; if (--o->refs == 0) o->Delete(); Object = 0; } } };
struct Building { int alive; long ForceOpenCount; };
struct Thing { int dead; int isFollower; int quit; };
struct World {
    Def* def; int found; int posted; Thing* hero; Thing* perceived; int opinion; int opinionSeen;
    int calls; int results[4]; int terminated; int someoneElse;
};
struct Group {
    World* w; Building* home; Building* ForceOpenBuilding; bool AllowDuplicate; bool GuardTold; long Handle;
    bool GetDef(DefPtr* out) { if (!w->found) return false; out->Object = w->def; ++w->def->refs; return true; }
    int FindAGuard() { return w->results[w->calls++]; }
    void Update()
    {
        ForceOpenBuilding = home;
        if (ForceOpenBuilding && ForceOpenBuilding->alive) ++ForceOpenBuilding->ForceOpenCount;
        if (AllowDuplicate || !w->someoneElse) {
            if (w->perceived) {
                DefPtr def;
                bool found = GetDef(&def);
                if (found) w->posted = (int)def.Object->refs;
                w->opinion = 1; w->opinionSeen = 1;
                Thing* hero = w->hero;
                if (hero && !hero->dead && w->perceived == hero && hero->isFollower) hero->quit = 1;
                int r;
                do { r = FindAGuard(); } while (r == 4);
                w->opinion = 0;
                if (r == 2) GuardTold = true;
            }
        }
        w->terminated = 1;
    }
};
int main()
{
    int ok = 1;
    Def d = { 1, 0 }; Building b = { 1, 3 }; Thing hero = { 0, 1, 0 };
    World w = { &d, 1, 0, &hero, &hero, 0, 0, 0, { 4, 4, 2, 0 }, 0, 1 };
    Group g = { &w, &b, 0, true, false, 7 };
    g.Update();
    ok &= b.ForceOpenCount == 4 && w.posted == 2 && d.refs == 1 && !d.deleted && hero.quit == 1 && w.calls == 3 && g.GuardTold
        && w.opinion == 0 && w.opinionSeen == 1 && w.terminated == 1;
    World w2 = { &d, 1, 0, 0, &hero, 0, 0, 0, { 1, 0, 0, 0 }, 0, 1 };
    Group g2 = { &w2, &b, 0, false, false, 7 };
    g2.Update();
    ok &= w2.calls == 0 && w2.terminated == 1 && !g2.GuardTold && b.ForceOpenCount == 5;
    Building dead = { 0, 9 };
    World w3 = { &d, 0, 0, 0, &hero, 0, 0, 0, { 3, 0, 0, 0 }, 0, 0 };
    Group g3 = { &w3, &dead, 0, false, false, 7 };
    g3.Update();
    ok &= dead.ForceOpenCount == 9 && w3.posted == 0 && w3.calls == 1 && !g3.GuardTold && w3.terminated;
    if (!ok) { printf("00958840_TEST FAIL\n"); return 1; }
    printf("00958840_TEST PASS\n");
    return 0;
}