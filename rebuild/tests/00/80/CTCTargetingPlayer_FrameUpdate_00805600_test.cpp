// Behaviour test for CTCTargetingPlayer::FrameUpdate (0x00805600) with its same-TU PostProcessTargetedThing (0x00803B20): forced-no-target
// clears the target and stops; while the Z-target toggle is held in Z-targeting mode the lock-selection timer counts queued
// TOGGLE_DEACTIVATE_LOCK_TARGET events (else resets); a live targeted thing is lost when it is in limbo, has no targeted component, or
// (Z-targeting, parent without TCI_WILL_LIGHTNING) is not Z-targetable -- losing clears the target point (index -1), the target and a pending
// TARGET_NEXT_NULL request; positions age; the target list is rebuilt when (frame + id) % 4 == 0 or a next-target request is pending; the
// projectile target is cleared and, first-person, re-targeted; with no target the NPC name display is cleared; then the nearest-interesting
// update, the threatening update when the target differs from the last threatened thing, and the creature check.
#include <cstdio>
#include <cstring>
struct Thing { bool dead, limbo, has_targeted, ztargetable; };
struct TP {
    bool forced, toggle, ztargeting, queued, first_person, lightning, next_thing; long method; long frame, id; long timer;
    Thing* target; Thing* point_thing; long point_index; float pos[3], old_pos[3], off[3], old_off[3];
    void* last_threatened; int list_updates, proj_updates, walk_updates, name_clears, nearest, threat_updates, creature_checks, base_updates;
    bool projectile_set;
    void FrameUpdate()
    {
        ++base_updates;
        if (forced) { target = 0; return; }
        if (toggle && ztargeting) { if (queued) ++timer; else timer = 0; }
        bool z_targeting = ztargeting;
        const Thing* ptargeted_thing = target;
        if (ptargeted_thing && !ptargeted_thing->dead) {
            bool lose_target = true;
            if (!ptargeted_thing->limbo && ptargeted_thing->has_targeted) {
                if (!z_targeting || lightning || ptargeted_thing->ztargetable) lose_target = false;
            }
            if (lose_target) {
                point_thing = 0; point_index = -1; target = 0;
                if (next_thing && method == 0) next_thing = false;
            }
        }
        std::memcpy(old_pos, pos, sizeof(pos)); std::memcpy(old_off, off, sizeof(off));
        long test_slong = frame + id;
        if (test_slong % 4 == 0 || next_thing) ++list_updates;
        projectile_set = false;
        if (first_person) { ++proj_updates; ++walk_updates; }
        if (!target) ++name_clears;
        ++nearest;
        if (target != last_threatened) ++threat_updates;
        ++creature_checks;
    }
};
static int fail(int n) { std::printf("00805600_TEST FAIL %d\n", n); return 1; }
static void reset(TP& t, Thing* th) { std::memset(&t, 0, sizeof(t)); t.target = th; t.point_thing = th; t.point_index = 3; t.last_threatened = th; t.frame = 1; t.id = 2; }
int main()
{
    Thing a = { false, false, true, true };
    TP t; reset(t, &a);
    t.forced = true; t.FrameUpdate();
    if (t.target || t.nearest || t.base_updates != 1) return fail(1);
    reset(t, &a); t.toggle = true; t.ztargeting = true; t.queued = true; t.timer = 5; t.FrameUpdate();
    if (t.timer != 6 || t.target != &a || t.point_index != 3) return fail(2);       // Z-targetable: kept
    reset(t, &a); t.toggle = true; t.ztargeting = true; t.queued = false; t.timer = 5; t.FrameUpdate();
    if (t.timer != 0) return fail(3);
    reset(t, &a); t.toggle = false; t.ztargeting = true; t.queued = true; t.timer = 5; t.FrameUpdate();
    if (t.timer != 5) return fail(4);
    reset(t, &a); t.toggle = true; t.ztargeting = false; t.queued = true; t.timer = 5; t.FrameUpdate();
    if (t.timer != 5) return fail(5);
    Thing b = { false, false, true, false };                                        // not Z-targetable
    reset(t, &b); t.ztargeting = true; t.next_thing = true; t.method = 0; t.FrameUpdate();
    if (t.target || t.point_thing || t.point_index != -1 || t.next_thing || t.name_clears != 1) return fail(6);
    reset(t, &b); t.ztargeting = false; t.FrameUpdate();                            // not Z-targeting: kept
    if (t.target != &b || t.name_clears) return fail(7);
    reset(t, &b); t.ztargeting = true; t.lightning = true; t.FrameUpdate();         // lightning: kept
    if (t.target != &b) return fail(8);
    reset(t, &b); t.ztargeting = true; t.next_thing = true; t.method = 2; t.FrameUpdate();
    if (t.target || !t.next_thing) return fail(9);                                  // other method keeps the request
    Thing c = { false, true, true, true };                                          // in limbo: lost
    reset(t, &c); t.FrameUpdate();
    if (t.target) return fail(10);
    Thing d = { false, false, false, true };                                        // no targeted component: lost
    reset(t, &d); t.FrameUpdate();
    if (t.target) return fail(11);
    Thing e = { true, true, false, false };                                         // dead: left alone
    reset(t, &e); t.FrameUpdate();
    if (t.target != &e || t.point_index != 3) return fail(12);
    reset(t, &a); t.pos[0] = 1; t.off[2] = 2; t.frame = 1; t.id = 3; t.FrameUpdate();   // (1+3)%4 == 0
    if (t.old_pos[0] != 1 || t.old_off[2] != 2 || t.list_updates != 1) return fail(13);
    reset(t, &a); t.frame = 1; t.id = 2; t.FrameUpdate();                           // 3
    if (t.list_updates) return fail(14);
    reset(t, &a); t.frame = -9; t.id = 2; t.FrameUpdate();                          // -7 % 4 = -3
    if (t.list_updates) return fail(15);
    reset(t, &a); t.frame = -9; t.id = 1; t.FrameUpdate();                          // -8 % 4 = 0
    if (t.list_updates != 1) return fail(16);
    reset(t, &a); t.frame = 1; t.id = 1; t.FrameUpdate();                           // 2: even but not a multiple of 4
    if (t.list_updates) return fail(22);
    reset(t, &a); t.frame = -9; t.id = 3; t.FrameUpdate();                          // -6 % 4 = -2
    if (t.list_updates) return fail(23);
    reset(t, &a); t.frame = 2; t.id = 4; t.FrameUpdate();                           // 6
    if (t.list_updates) return fail(24);
    reset(t, &a); t.next_thing = true; t.FrameUpdate();
    if (t.list_updates != 1) return fail(17);
    reset(t, &a); t.projectile_set = true; t.FrameUpdate();                         // projectile target always cleared
    if (t.projectile_set) return fail(25);
    reset(t, &a); t.first_person = true; t.projectile_set = true; t.FrameUpdate();
    if (t.proj_updates != 1 || t.walk_updates != 1 || t.projectile_set) return fail(18);
    reset(t, &a); t.FrameUpdate();
    if (t.proj_updates || t.nearest != 1 || t.threat_updates || t.creature_checks != 1) return fail(19);
    reset(t, &a); t.last_threatened = &b; t.FrameUpdate();
    if (t.threat_updates != 1 || t.creature_checks != 1) return fail(20);
    reset(t, 0); t.FrameUpdate();
    if (t.name_clears != 1 || t.threat_updates) return fail(21);
    std::printf("00805600_TEST PASS\n");
    return 0;
}