// Behaviour test for CCreatureAction_GenericFireProjectileWeapon::PerformAction (0x008BF280): nothing unless the creature is alive
// (non-null, not dead); then a SIMPLE_NOISE; the carrying component's primary-slot thing must be alive; its weapon component (if any)
// is unloaded and its animating component (if any) plays PROJECTILE_WEAPON_FIRE; the shot index comes from the weapon component (a
// carried thing without one faults in the out-of-line CTCWeapon::GetCreatedShotDefGlobalIndex 0x005DA750, which reads [this+0Ch] first:
// modelled as a thrown fault, after the animation and before any shot); index 0 creates nothing, any other index (negative included:
// `test ebp,ebp ; je`, 0x008BF3F4) CreateShot(index, creature, carried, target, accuracy, 1, 1); a live shot gets
// ForceResendEveryRenderFrame and, if the creature has an entity-events component, one fired-shot event for the target.
#include <cstdio>
#include <cstring>
struct Weapon { int unloaded; long index; };
struct NullComponentFault { };                            // retail: access violation reading [0+0Ch] at 0x005DA754
static long GetCreatedShotDefGlobalIndex(const Weapon* w) { if (!w) throw NullComponentFault(); return w->index; }
struct Anim { int plays; char name[32]; };
struct Events { int added; const void* target; };
struct Appearance { int force_resend; };
struct Thing {
    bool dead; Weapon* weapon; Anim* anim; Events* events; Thing* carried; Appearance* app; long accuracy;
};
struct Log { int noises; int shots; long index; Thing* shooter; Thing* weapon; Thing* target; float acc, dmg, build; Thing* result; };
static Log g;
static bool alive(const Thing* t) { return t != 0 && !t->dead; }
static Thing* CreateShot(long index, Thing* shooter, Thing* weapon, Thing* target, float acc, float dmg, float build)
{
    ++g.shots; g.index = index; g.shooter = shooter; g.weapon = weapon; g.target = target; g.acc = acc; g.dmg = dmg; g.build = build;
    return g.result;
}
static void PerformAction(Thing* creature, Thing* target)
{
    if (alive(creature)) {
        ++g.noises;
        Weapon* ptc_weapon = 0;
        Thing* pcarried_thing = creature->carried;
        if (alive(pcarried_thing)) {
            if (pcarried_thing->weapon) { ptc_weapon = pcarried_thing->weapon; ptc_weapon->unloaded++; }
            if (pcarried_thing->anim) { pcarried_thing->anim->plays++; std::strcpy(pcarried_thing->anim->name, "PROJECTILE_WEAPON_FIRE"); }
            float shot_accuracy_percentage = (float)creature->accuracy;
            long global_index = GetCreatedShotDefGlobalIndex(ptc_weapon);
            float shot_damage_multiplier = 1.0f;
            if (global_index) {
                Thing* pshot = CreateShot(global_index, creature, pcarried_thing, target, shot_accuracy_percentage, shot_damage_multiplier, 1.0f);
                if (alive(pshot)) {
                    pshot->app->force_resend = 1;
                    if (creature->events) { creature->events->added++; creature->events->target = target; }
                }
            }
        }
    }
}
static int fail(int n) { std::printf("008bf280_TEST FAIL %d\n", n); return 1; }
int main()
{
    Weapon wpn = { 0, 7 }; Anim anim; std::memset(&anim, 0, sizeof(anim)); Events ev = { 0, 0 }; Appearance app = { 0 };
    Thing shot; std::memset(&shot, 0, sizeof(shot)); shot.app = &app;
    Thing bow; std::memset(&bow, 0, sizeof(bow)); bow.weapon = &wpn; bow.anim = &anim;
    Thing tgt; std::memset(&tgt, 0, sizeof(tgt));
    Thing cr; std::memset(&cr, 0, sizeof(cr)); cr.carried = &bow; cr.events = &ev; cr.accuracy = 85;
    std::memset(&g, 0, sizeof(g)); g.result = &shot;
    PerformAction(0, &tgt);                                   // null creature
    if (g.noises || g.shots) return fail(1);
    cr.dead = true; PerformAction(&cr, &tgt); cr.dead = false; // dead creature
    if (g.noises || g.shots) return fail(2);
    PerformAction(&cr, &tgt);                                 // full path
    if (g.noises != 1 || wpn.unloaded != 1 || anim.plays != 1 || std::strcmp(anim.name, "PROJECTILE_WEAPON_FIRE")) return fail(3);
    if (g.shots != 1 || g.index != 7 || g.shooter != &cr || g.weapon != &bow || g.target != &tgt) return fail(4);
    if (g.acc != 85.0f || g.dmg != 1.0f || g.build != 1.0f) return fail(5);
    if (app.force_resend != 1 || ev.added != 1 || ev.target != &tgt) return fail(6);
    bow.dead = true; PerformAction(&cr, &tgt); bow.dead = false; // dead carried thing: noise only
    if (g.noises != 2 || g.shots != 1 || wpn.unloaded != 1) return fail(7);
    cr.carried = 0; PerformAction(&cr, &tgt); cr.carried = &bow; // nothing carried
    if (g.noises != 3 || g.shots != 1) return fail(8);
    bow.anim = 0; PerformAction(&cr, &tgt); bow.anim = &anim;  // no animating component: no animation, shot still made
    if (anim.plays != 1 || g.shots != 2 || wpn.unloaded != 2 || ev.added != 2) return fail(9);
    wpn.index = 0; PerformAction(&cr, &tgt); wpn.index = 7;    // shot index 0: no shot
    if (g.shots != 2 || wpn.unloaded != 3 || anim.plays != 2) return fail(10);
    g.result = 0; app.force_resend = 0; PerformAction(&cr, &tgt); // no shot created
    if (g.shots != 3 || app.force_resend || ev.added != 2) return fail(11);
    shot.dead = true; g.result = &shot; PerformAction(&cr, &tgt); shot.dead = false; // dead shot
    if (g.shots != 4 || app.force_resend || ev.added != 2) return fail(12);
    cr.events = 0; PerformAction(&cr, &tgt); cr.events = &ev;  // no events component: shot flagged, no event
    if (g.shots != 5 || app.force_resend != 1 || ev.added != 2) return fail(13);
    bool faulted = false;                                      // no weapon component: nothing unloaded, the animation plays, then the
    bow.weapon = 0;                                            // null component's getter faults before any shot
    try { PerformAction(&cr, &tgt); } catch (NullComponentFault&) { faulted = true; }
    bow.weapon = &wpn;
    if (!faulted || wpn.unloaded != 6 || anim.plays != 6 || g.shots != 5) return fail(14);
    wpn.index = -1; PerformAction(&cr, &tgt); wpn.index = 7;   // negative index: still a shot (only 0 is skipped)
    if (g.shots != 6 || g.index != -1 || ev.added != 3) return fail(15);
    std::printf("008bf280_TEST PASS\n");
    return 0;
}