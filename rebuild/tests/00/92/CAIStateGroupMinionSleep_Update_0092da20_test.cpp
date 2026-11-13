// Behaviour test for 0x0092DA20 CAIStateGroup_MinionSleep::Update() -- a host model of the row's logic (same control flow, same
// decisions) over a scripted creature / bed / world, checked against the outcomes retail's bytes produce.
#include <cstdio>
#include <cstring>

// ---- scripted environment -------------------------------------------------------------------------------------------------
struct World {
    bool free_bed, free_bandit_bed;
    bool home_alive_at_start;
    int home_alive_until_iteration;          // home building dies at this loop iteration (-1: never)
    bool has_sleep_tc;
    bool bed_to_right;
    bool has_get_in_anim;                    // HasAnim("BED_GET_IN_<side>")
    long anim_get_in, anim_get_out, anim_get_out_fast, anim_sleep;
    bool fast_track;
    bool carrying_weapon;
    bool has_mode_manager;
    const char* fast_name;                   // CBedDef::GetUpFromBedFastAnimName ("" = none)
    int do_nothing_continues;                // AIAction_DoNothing returns 4 this many times
    int sleep_time_until;                    // IsInSleepTime false from this iteration on (-1: never)
    int ready_until;                         // GetIsReadyToSleep false from this iteration on (-1: never)
    int trespass_at;                         // IsHeroTrespassingPerceived true at this iteration (-1: never)
    bool bed_alive;
    bool bed_has_tc;
    long get_out_result;                     // PerformActionAndWait(GetOutOfBed) result
};

struct Log {
    int terminated;
    char side[8];
    long fast_anim_member;
    int sheathe_all, remove_mode4, reset_pos, reset_rh, interrupt_nav;
    int sheathe_action, unsheathe_action;
    long get_in_anim, sleep_anim;
    int get_out_fast_path;                   // 1 fast name used, 0 normal name
    long bed_anim_started;                   // StartGetOutOfBedAnimation argument (0 = not called)
    bool play_emergency;
    bool was_carrying;
    int iterations;
};

class MinionSleepModel {
public:
    MinionSleepModel(const World& w, Log& log) : W(w), L(log), PlayEmergencyGetOutOfBed(false), BedGetOutOfBedFastAnim(-7),
        WasCarryingWeapon(false), Iteration(0) { std::memset(&L, 0, sizeof(L)); }
    void Update();
private:
    bool IsInSleepTime() const { return W.sleep_time_until < 0 || Iteration < W.sleep_time_until; }
    bool IsHeroTrespassingPerceived() const { return Iteration == W.trespass_at; }
    bool HomeAlive() const { return W.home_alive_until_iteration < 0 || Iteration < W.home_alive_until_iteration; }
    void Terminate() { ++L.terminated; L.play_emergency = PlayEmergencyGetOutOfBed; L.was_carrying = WasCarryingWeapon;
                       L.fast_anim_member = BedGetOutOfBedFastAnim; }
    int DoNothing() { if (Iteration < W.do_nothing_continues) return 4; return 2; }
    const World& W;
    Log& L;
    bool PlayEmergencyGetOutOfBed;
    long BedGetOutOfBedFastAnim;
    bool WasCarryingWeapon;
    int Iteration;
};

void MinionSleepModel::Update()
{
    if (!W.free_bed && !W.free_bandit_bed) {
        Terminate();
        return;
    }
    bool home_alive = W.home_alive_at_start;
    if (!W.has_sleep_tc) {
        Terminate();
        return;
    }
    PlayEmergencyGetOutOfBed = false;
    if (W.bed_to_right)
        std::strcpy(L.side, "RIGHT");
    else
        std::strcpy(L.side, "LEFT");
    BedGetOutOfBedFastAnim = 0;
    long get_in_anim = 0, get_out_anim = 0, get_out_fast_anim = 0, sleep_anim = 0;
    if (W.has_get_in_anim) {
        get_in_anim = W.anim_get_in;
        get_out_anim = W.anim_get_out;
        get_out_fast_anim = W.anim_get_out_fast;
        if (!get_out_fast_anim)
            get_out_fast_anim = get_out_anim;
        sleep_anim = W.anim_sleep;
        BedGetOutOfBedFastAnim = get_out_fast_anim;
    }
    if (W.fast_track) {
        WasCarryingWeapon = W.carrying_weapon;
        if (WasCarryingWeapon) {
            ++L.sheathe_all;
            if (W.has_mode_manager)
                ++L.remove_mode4;
        }
        ++L.reset_pos;
        ++L.reset_rh;
        ++L.interrupt_nav;
    }
    WasCarryingWeapon = W.carrying_weapon;
    if (WasCarryingWeapon)
        ++L.sheathe_action;
    PlayEmergencyGetOutOfBed = true;
    L.get_in_anim = get_in_anim;
    L.sleep_anim = sleep_anim;
    bool disturbed = true;
    while (DoNothing() == 4) {
        if (!IsInSleepTime()) {
            disturbed = false;
            break;
        }
        if (Iteration >= W.ready_until && W.ready_until >= 0)
            break;
        if (IsHeroTrespassingPerceived())
            break;
        if (home_alive && !HomeAlive()) {
            disturbed = false;
            break;
        }
        ++Iteration;
    }
    L.iterations = Iteration;
    long anim;
    if (disturbed && W.fast_name[0] != 0) {
        L.get_out_fast_path = 1;
        anim = get_out_fast_anim;
    } else {
        L.get_out_fast_path = 0;
        anim = get_out_anim;
    }
    if (W.bed_alive && W.bed_has_tc && anim)
        L.bed_anim_started = anim;
    if (W.get_out_result == 2)
        PlayEmergencyGetOutOfBed = false;
    if (WasCarryingWeapon) {
        ++L.unsheathe_action;
        WasCarryingWeapon = false;
    }
    Terminate();
}

static World Base()
{
    World w;
    w.free_bed = true; w.free_bandit_bed = false;
    w.home_alive_at_start = true; w.home_alive_until_iteration = -1;
    w.has_sleep_tc = true; w.bed_to_right = true; w.has_get_in_anim = true;
    w.anim_get_in = 11; w.anim_get_out = 12; w.anim_get_out_fast = 13; w.anim_sleep = 14;
    w.fast_track = false; w.carrying_weapon = false; w.has_mode_manager = true;
    w.fast_name = "BED_FAST"; w.do_nothing_continues = 100; w.sleep_time_until = 5; w.ready_until = -1; w.trespass_at = -1;
    w.bed_alive = true; w.bed_has_tc = true; w.get_out_result = 2;
    return w;
}

static int Check(bool ok, int line)
{
    if (!ok) std::printf("check failed at line %d\n", line);
    return ok ? 0 : 1;
}
#define CHECK(x) fails += Check((x), __LINE__)

int main()
{
    int fails = 0;
    Log l;
    // 1. no bed at all: terminate straight away
    { World w = Base(); w.free_bed = false; MinionSleepModel m(w, l); m.Update();
      CHECK(l.terminated == 1 && l.side[0] == 0 && l.fast_anim_member == -7); }
    // 2. no sleep component: terminate, bed side untouched
    { World w = Base(); w.has_sleep_tc = false; w.free_bed = false; w.free_bandit_bed = true; MinionSleepModel m(w, l); m.Update();
      CHECK(l.terminated == 1 && l.side[0] == 0); }
    // 3. sleep time runs out: normal get-out with the plain get-out anim, emergency flag cleared on success
    { World w = Base(); MinionSleepModel m(w, l); m.Update();
      CHECK(l.terminated == 1 && std::strcmp(l.side, "RIGHT") == 0 && l.get_in_anim == 11 && l.sleep_anim == 14);
      CHECK(l.get_out_fast_path == 0 && l.bed_anim_started == 12 && !l.play_emergency && l.iterations == 5 && l.fast_anim_member == 13); }
    // 4. no fast anim on the creature: falls back to the get-out anim; disturbed (not ready) -> fast path with that index
    { World w = Base(); w.anim_get_out_fast = 0; w.ready_until = 2; w.bed_to_right = false; MinionSleepModel m(w, l); m.Update();
      CHECK(std::strcmp(l.side, "LEFT") == 0 && l.fast_anim_member == 12 && l.get_out_fast_path == 1 && l.bed_anim_started == 12); }
    // 5. hero trespassing: fast path, fast anim; get-out fails -> emergency flag stays set
    { World w = Base(); w.trespass_at = 3; w.get_out_result = 4; MinionSleepModel m(w, l); m.Update();
      CHECK(l.get_out_fast_path == 1 && l.bed_anim_started == 13 && l.play_emergency && l.iterations == 3); }
    // 6. disturbed but the def has no fast get-up name: normal path
    { World w = Base(); w.trespass_at = 1; w.fast_name = ""; MinionSleepModel m(w, l); m.Update();
      CHECK(l.get_out_fast_path == 0 && l.bed_anim_started == 12); }
    // 7. home building destroyed while asleep: normal path even with a fast name
    { World w = Base(); w.home_alive_until_iteration = 2; MinionSleepModel m(w, l); m.Update();
      CHECK(l.get_out_fast_path == 0 && l.iterations == 2); }
    // 8. home was already dead at the start: its death is ignored
    { World w = Base(); w.home_alive_at_start = false; w.home_alive_until_iteration = 0; MinionSleepModel m(w, l); m.Update();
      CHECK(l.iterations == 5 && l.get_out_fast_path == 0); }
    // 9. DoNothing stops (state interrupted): disturbed -> fast path
    { World w = Base(); w.do_nothing_continues = 1; MinionSleepModel m(w, l); m.Update();
      CHECK(l.get_out_fast_path == 1 && l.iterations == 1); }
    // 10. fast-tracked while carrying a weapon: sheathe all, drop mode 4, reset to the bed, interrupt navigation, sheathe action,
    //     unsheathe at the end and clear the flag
    { World w = Base(); w.fast_track = true; w.carrying_weapon = true; MinionSleepModel m(w, l); m.Update();
      CHECK(l.sheathe_all == 1 && l.remove_mode4 == 1 && l.reset_pos == 1 && l.reset_rh == 1 && l.interrupt_nav == 1);
      CHECK(l.sheathe_action == 1 && l.unsheathe_action == 1 && !l.was_carrying); }
    // 11. creature without the bed animations: all indices 0, no bed animation started
    { World w = Base(); w.has_get_in_anim = false; MinionSleepModel m(w, l); m.Update();
      CHECK(l.get_in_anim == 0 && l.sleep_anim == 0 && l.fast_anim_member == 0 && l.bed_anim_started == 0); }
    // 12. dead bed: no get-out animation on the bed
    { World w = Base(); w.bed_alive = false; MinionSleepModel m(w, l); m.Update();
      CHECK(l.bed_anim_started == 0 && l.terminated == 1); }
    if (fails) { std::printf("0092da20_TEST FAIL\n"); return 1; }
    std::printf("0092da20_TEST PASS\n");
    return 0;
}