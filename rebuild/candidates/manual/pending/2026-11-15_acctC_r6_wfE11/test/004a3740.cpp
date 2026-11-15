// Behaviour test for CWorld::UpdateRegionLoad @ 0x004A3740: the region-load state machine's decisions, through a copy of the row's
// control flow over mocks (same statements, order and conditions as src/004a3740.cpp):
//  - case 1 quest-region confirmation: when the leave-region text is shown (quest screens, between quest screens, no cutscene, no
//    movie, region not valid for the locking mode), the yes / no answer (yes = 0: confirmed, fade out unless via teleporter; no = 1:
//    cancel, status 0, AutoLoadRegionLeaving), the teleporter path (returns until confirmed, then resets the start position / angle),
//    the door path (waits for the full fade, puts the hero at the nearest region entrance, resets, ends the fade, unfreezes);
//  - case 4 guild-seal recall target: entrance v teleporter by 2D squared distance (the nearer one, the entrance on a tie), the
//    facing angle of the entrance or of the hero, the first-entrance fallback, the angle only stored when the target has physics;
//  - case 6 follower placement: directed position, else wander position, else start + direction; the direction turned by
//    angle_step after EVERY follower, dead / missing followers skipped (no turn);
//  - case 8 save-game marker: set when quest screens are off, or when not between quest screens and the region does not start the
//    locking mode;
//  - case 9: RemoveMode(PLAYER_MODE_FREEZE_CONTROLS) once per counted AddMode, then the count reset.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
static int gFail = 0;
static void Check(bool c, const char* what) { if (!c) { std::printf("FAIL: %s\n", what); gFail = 1; } }
struct V3 { float x, y, z; V3(float a = 0, float b = 0, float c = 0) : x(a), y(b), z(c) {} };
static float Dist2D2(const V3& a, const V3& b) { float dx = b.x - a.x, dy = b.y - a.y; return dy * dy + dx * dx; }

// ---- case 1 -------------------------------------------------------------------------------------------------------------
struct C1 {
    bool questScreens, waiting, between, cutscene, movie, regionValid, viaTeleporter, fadeOut, fullyFaded, frozen, entrance;
    int answer;                       // -1 none, else the message's ExtraData
    long status; V3 startPos; float startAngle;
    std::string log;
};
static void Case1Confirm(C1& w)      // returns like the row: every exit of this block is a `return`
{
    if (!w.questScreens) { w.log += "noquest;"; return; }
    if (!w.waiting && w.between && !w.cutscene && !w.movie) {
        if (!w.regionValid) { w.log += "SetText;"; w.waiting = true; }
    }
    bool confirmed = false;
    if (w.waiting) {
        if (!w.fadeOut) {
            if (w.answer >= 0) {
                if (w.answer == 1) { w.waiting = false; w.status = 0; w.log += "AutoLoadRegionLeaving;"; }
                else if (w.answer == 0) {
                    confirmed = true;
                    if (!w.viaTeleporter) {
                        if (!w.frozen) { w.frozen = true; w.log += "Freeze;"; }
                        w.log += "StartFadeOut;";
                    }
                }
                w.log += "RemoveMessage;";
            }
        }
        if (w.viaTeleporter) {
            if (!confirmed) return;
            w.waiting = false; w.status = 0; w.startPos = V3(); w.startAngle = 0.0f;
            return;
        }
        if (!w.fullyFaded) return;
        if (w.entrance) w.log += "ResetToEntrance;";
        w.waiting = false; w.status = 0; w.startPos = V3(); w.startAngle = 0.0f;
        w.log += "EndFadeOut;Unfreeze;";
        return;
    }
    w.log += "continue;";
}

// ---- case 4 -------------------------------------------------------------------------------------------------------------
struct Thing { V3 pos; bool physics; float facing; };
struct Recall { bool set, angleSet; V3 pos; float angle; };
static Recall Case4Recall(const Thing& hero, const Thing* entrance, const Thing* teleporter, const Thing* firstEntrance)
{
    const Thing* precall_thing = 0; float angle = 0.0f;
    if (entrance) {
        if (teleporter) {
            if (Dist2D2(hero.pos, entrance->pos) > Dist2D2(hero.pos, teleporter->pos)) { precall_thing = teleporter; angle = hero.facing; }
            else { precall_thing = entrance; angle = entrance->facing; }
        } else { precall_thing = entrance; angle = entrance->facing; }
    } else if (teleporter) { precall_thing = teleporter; angle = hero.facing; }
    else if (firstEntrance) { precall_thing = firstEntrance; angle = hero.facing; }
    Recall r = { false, false, V3(), 0.0f };
    if (precall_thing) {
        r.set = true; r.pos = precall_thing->pos;
        if (precall_thing->physics) { r.angleSet = true; r.angle = angle; }
    }
    return r;
}

// ---- case 6 -------------------------------------------------------------------------------------------------------------
struct Follower { bool alive, nav, directedOk, wanderOk; V3 placed; float dirAtPlace; };
static void RotateXY(V3& v, float a) { float c = std::cos(a), s = std::sin(a); float x = v.x * c - v.y * s; v.y = v.x * s + v.y * c; v.x = x; }
static void Case6Place(std::vector<Follower>& f, const V3& start, V3 direction, float angle_step)
{
    RotateXY(direction, 0.25f);
    for (unsigned long i = 0; i < f.size(); ++i) {
        Follower& p = f[i];
        if (!p.alive) continue;
        V3 nearby_pos(99, 99, 99);
        bool has_navigation = p.nav;
        if (has_navigation && p.directedOk) { p.placed = nearby_pos; p.dirAtPlace = direction.x; RotateXY(direction, angle_step); }
        else if (has_navigation && p.wanderOk) { p.placed = nearby_pos; p.dirAtPlace = direction.x; RotateXY(direction, angle_step); }
        else { p.placed = V3(start.x + direction.x, start.y + direction.y, start.z + direction.z); p.dirAtPlace = direction.x; RotateXY(direction, angle_step); }
    }
}

// ---- case 8 / case 9 ----------------------------------------------------------------------------------------------------
static bool Case8SetsMarker(bool questScreens, bool between, bool regionStartsLocking)
{
    bool set = false;
    if (!questScreens) set = true;
    else if (!between && !regionStartsLocking) set = true;
    return set;
}
static long Case9Unfreeze(long& added) { long removed = 0; for (long i = 0; i < added; ++i) ++removed; added = 0; return removed; }

int main()
{
    // case 1
    C1 base = { true, false, true, false, false, false, false, false, false, false, true, -1, 1, V3(5, 6, 7), 1.5f, "" };
    C1 w = base; Case1Confirm(w);
    Check(w.waiting && w.log == "SetText;", "invalid quest region: text shown, waiting, no answer yet -> return");
    w = base; w.regionValid = true; Case1Confirm(w);
    Check(!w.waiting && w.log == "continue;", "valid region: no text, region load goes on");
    w = base; w.cutscene = true; Case1Confirm(w);
    Check(w.log == "continue;", "cutscene: no text");
    w = base; w.waiting = true; w.answer = 1; Case1Confirm(w);
    Check(!w.waiting && w.status == 0 && w.log == "AutoLoadRegionLeaving;RemoveMessage;", "answer no (1): cancel, status 0");
    w = base; w.waiting = true; w.answer = 0; Case1Confirm(w);
    Check(w.frozen && w.status == 1 && w.log == "Freeze;StartFadeOut;RemoveMessage;", "answer yes (0) via door: freeze + fade, wait for the fade");
    w = base; w.waiting = true; w.answer = 0; w.frozen = true; Case1Confirm(w);
    Check(w.log == "StartFadeOut;RemoveMessage;", "already frozen: no second AddMode");
    w = base; w.waiting = true; w.answer = 0; w.viaTeleporter = true; Case1Confirm(w);
    Check(!w.waiting && w.status == 0 && w.startPos.x == 0 && w.startAngle == 0 && w.log == "RemoveMessage;", "teleporter + yes: reset, no fade");
    w = base; w.waiting = true; w.viaTeleporter = true; Case1Confirm(w);
    Check(w.waiting && w.status == 1 && w.startPos.x == 5, "teleporter, no answer: return unchanged");
    w = base; w.waiting = true; w.fullyFaded = true; Case1Confirm(w);
    Check(!w.waiting && w.status == 0 && w.startAngle == 0 && w.log == "ResetToEntrance;EndFadeOut;Unfreeze;", "door, faded: hero to entrance, reset");
    w = base; w.waiting = true; w.fadeOut = true; w.answer = 0; Case1Confirm(w);
    Check(w.log == "", "fading: the answer is not read");
    // case 4
    Thing hero = { V3(0, 0, 0), true, 0.7f };
    Thing ent = { V3(3, 0, 50), true, 1.1f }, tel = { V3(0, 4, -50), true, 2.2f }, noPhys = { V3(1, 1, 1), false, 3.3f };
    Recall r = Case4Recall(hero, &ent, &tel, 0);
    Check(r.set && r.pos.x == 3 && r.angleSet && r.angle == 1.1f, "entrance nearer in 2D (z ignored): entrance + its facing");
    Thing tel2 = { V3(0, 2, 0), true, 2.2f };
    r = Case4Recall(hero, &ent, &tel2, 0);
    Check(r.set && r.pos.y == 2 && r.angle == 0.7f, "teleporter nearer: teleporter + the hero's facing");
    Thing tie = { V3(0, 3, 0), true, 2.2f };
    r = Case4Recall(hero, &ent, &tie, 0);
    Check(r.pos.x == 3 && r.angle == 1.1f, "equal distance: the entrance");
    r = Case4Recall(hero, 0, 0, &noPhys);
    Check(r.set && r.pos.x == 1 && !r.angleSet, "first-entrance fallback without physics: position only");
    r = Case4Recall(hero, 0, 0, 0);
    Check(!r.set, "nothing found: no recall position");
    // case 6
    std::vector<Follower> f(4);
    Follower a = { true, true, true, false, V3(), 0 }, b = { true, true, false, true, V3(), 0 }, c = { false, true, true, true, V3(), 0 }, d = { true, false, true, true, V3(), 0 };
    f[0] = a; f[1] = b; f[2] = c; f[3] = d;
    Case6Place(f, V3(10, 20, 30), V3(1, 0, 0), 0.5f / 4.0f);
    Check(f[0].placed.x == 99 && f[1].placed.x == 99, "directed / wander positions used");
    Check(f[2].placed.x == 0 && f[2].dirAtPlace == 0, "dead follower skipped");
    float d0 = std::cos(0.25f), d3 = std::cos(0.25f + 2 * 0.125f);
    Check(std::fabs(f[0].dirAtPlace - d0) < 1e-5f, "first follower at the start direction turned by 0.25");
    Check(std::fabs(f[3].dirAtPlace - d3) < 1e-5f && std::fabs(f[3].placed.x - (10 + d3)) < 1e-5f,
          "no navigation: start + direction, the direction turned once per placed follower");
    // case 8 / 9
    Check(Case8SetsMarker(false, true, true), "quest screens off: marker set");
    Check(Case8SetsMarker(true, false, false), "not between, region not locking: marker set");
    Check(!Case8SetsMarker(true, true, false) && !Case8SetsMarker(true, false, true), "between quest screens / locking region: no marker");
    long added = 3; long removed = Case9Unfreeze(added);
    Check(removed == 3 && added == 0, "one RemoveMode per AddMode, count reset");
    added = 0; Check(Case9Unfreeze(added) == 0, "no AddMode: no RemoveMode");
    if (gFail) { std::printf("004a3740_TEST FAIL\n"); return 1; }
    std::printf("004a3740_TEST PASS\n");
    return 0;
}
