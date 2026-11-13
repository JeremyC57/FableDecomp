// Behaviour test for 0x0094E7B0 CAIStateGroup_EarthTroll::Update() -- a host model of the row's decisions (same control flow) over a
// scripted troll / enemy / world, checked against the outcomes retail's bytes produce.
#include <cstdio>
#include <cmath>
#include <cstring>

struct Vec { float X, Y, Z; };

struct Script {
    int runnable_until;              // IsStillRunnable true for this many calls (-1: always)
    bool enemy_alive;
    float dist_first;                // 2D distance to the predicted position at the start
    float dist_after_turn;           // ... after turning
    float dist_after_pound;          // ... to the enemy after the close-range arm
    float facing_dot;                // facing . direction
    float right_dot;                 // (facing rotated -0.25) . direction
    int throw_results[8];            // ThrowRocks results per volley
    float throw_dist[8];             // distance after each throw
    int knock_on_throw;              // HasEnemyBeenKnockedDown true on this throw index (-1: never)
    bool knocked_in_pound;
    bool stopping;
    long frames_since_knockdown;
    float min_laugh, max_boast_time;
    float max_dist, min_dist;
};

struct Log {
    int terminated, turned_ac90, turned_c90, pounds, look_at, throws, idles_played, idle_actions, boasts, combo_resets;
    long shot_counter;
};

class TrollModel {
public:
    TrollModel(const Script& s, Log& l) : S(s), L(l), Calls(0), ShotCounter(-1), Throw(0) { std::memset(&L, 0, sizeof(L)); }
    void Update();
private:
    bool IsStillRunnable() { ++Calls; return S.runnable_until < 0 || Calls <= S.runnable_until; }
    void Terminate() { ++L.terminated; L.shot_counter = ShotCounter; }
    const Script& S;
    Log& L;
    int Calls;
    long ShotCounter;
    int Throw;
};

void TrollModel::Update()
{
    if (!IsStillRunnable()) { Terminate(); return; }
    bool knocked_down = false;
    if (S.enemy_alive) {
        if (!IsStillRunnable()) { Terminate(); return; }
        float distance = S.dist_first;
        if (distance < S.min_dist) {
            if (S.facing_dot < 0.5f) {
                if (S.right_dot > 0.0f) ++L.turned_ac90; else ++L.turned_c90;
                if (!IsStillRunnable()) { Terminate(); return; }
                distance = S.dist_after_turn;
            }
            if (distance < S.min_dist) {
                ++L.pounds;
                if (S.knocked_in_pound) knocked_down = true;
                if (!IsStillRunnable()) { Terminate(); return; }
            }
            distance = S.dist_after_pound;
        } else if (distance <= S.max_dist) {
            ++L.look_at;
            if (!IsStillRunnable()) { Terminate(); return; }
            ShotCounter = 0;
            int result;
            do {
                if (ShotCounter == 0) ++L.combo_resets;
                result = S.throw_results[Throw];
                ++ShotCounter;                         // ThrowRocks counts the shot
                ++L.throws;
                if (!IsStillRunnable()) { Terminate(); return; }
                if (Throw == S.knock_on_throw) knocked_down = true;
                if (ShotCounter == 2 && result == 2) {
                    ++L.idles_played;
                    ShotCounter = 0;
                    if (!knocked_down) { ++L.idle_actions; result = 2; }
                }
                if (!IsStillRunnable()) { Terminate(); return; }
                distance = S.throw_dist[Throw];
                ++Throw;
            } while ((result == 4 || result == 2) && distance >= S.min_dist && distance <= S.max_dist && !knocked_down && Throw < 8);
            if (distance > S.max_dist) { Terminate(); return; }
        } else {
            Terminate();
            return;
        }
        if (!S.stopping && distance >= S.min_laugh && knocked_down && (float)S.frames_since_knockdown < S.max_boast_time)
            ++L.boasts;
    } else
        Terminate();
    if (S.stopping) { Terminate(); return; }
}

static Script Base()
{
    Script s;
    std::memset(&s, 0, sizeof(s));
    s.runnable_until = -1; s.enemy_alive = true; s.min_dist = 5.0f; s.max_dist = 30.0f;
    s.dist_first = 3.0f; s.dist_after_turn = 3.0f; s.dist_after_pound = 4.0f; s.facing_dot = 0.9f; s.right_dot = 0.3f;
    s.knock_on_throw = -1; s.min_laugh = 2.0f; s.max_boast_time = 100.0f; s.frames_since_knockdown = 10;
    for (int i = 0; i < 8; ++i) { s.throw_results[i] = 2; s.throw_dist[i] = 10.0f; }
    return s;
}

static int Check(bool ok, int line) { if (!ok) std::printf("check failed at line %d\n", line); return ok ? 0 : 1; }
#define CHECK(x) fails += Check((x), __LINE__)

int main()
{
    int fails = 0;
    Log l;
    // not runnable at all
    { Script s = Base(); s.runnable_until = 0; TrollModel m(s, l); m.Update(); CHECK(l.terminated == 1 && l.pounds == 0); }
    // enemy dead: terminate once, no stopping
    { Script s = Base(); s.enemy_alive = false; TrollModel m(s, l); m.Update(); CHECK(l.terminated == 1 && l.look_at == 0); }
    // close and facing: pound without turning, no boast without a knock-down
    { Script s = Base(); TrollModel m(s, l); m.Update(); CHECK(l.pounds == 1 && l.turned_ac90 == 0 && l.turned_c90 == 0 && l.boasts == 0 && l.terminated == 0); }
    // close, not facing, enemy on the left (right_dot <= 0): TURNING_C90; knocked down in the pound -> boast
    { Script s = Base(); s.facing_dot = 0.1f; s.right_dot = -0.5f; s.knocked_in_pound = true; TrollModel m(s, l); m.Update();
      CHECK(l.turned_c90 == 1 && l.pounds == 1 && l.boasts == 1); }
    // turn (AC90) then the enemy moved out of pound range: no pound
    { Script s = Base(); s.facing_dot = 0.0f; s.right_dot = 0.5f; s.dist_after_turn = 6.0f; TrollModel m(s, l); m.Update();
      CHECK(l.turned_ac90 == 1 && l.pounds == 0); }
    // knocked down but too close to laugh, or too long ago: no boast
    { Script s = Base(); s.knocked_in_pound = true; s.dist_after_pound = 1.0f; TrollModel m(s, l); m.Update(); CHECK(l.boasts == 0); }
    { Script s = Base(); s.knocked_in_pound = true; s.frames_since_knockdown = 200; TrollModel m(s, l); m.Update(); CHECK(l.boasts == 0); }
    // out of range: terminate
    { Script s = Base(); s.dist_first = 40.0f; TrollModel m(s, l); m.Update(); CHECK(l.terminated == 1 && l.look_at == 0); }
    // throwing range: volleys of two with an idle pause in between, until the enemy leaves the range
    { Script s = Base(); s.dist_first = 10.0f; s.throw_dist[4] = 35.0f; TrollModel m(s, l); m.Update();
      CHECK(l.look_at == 1 && l.throws == 5 && l.idles_played == 2 && l.idle_actions == 2 && l.combo_resets == 3 && l.terminated == 1); }
    // throwing range: knock-down on the first throw stops the volley; boast
    { Script s = Base(); s.dist_first = 10.0f; s.knock_on_throw = 0; TrollModel m(s, l); m.Update();
      CHECK(l.throws == 1 && l.boasts == 1 && l.terminated == 0); }
    // stopping: terminate at the end, never boast
    { Script s = Base(); s.knocked_in_pound = true; s.stopping = true; TrollModel m(s, l); m.Update(); CHECK(l.boasts == 0 && l.terminated == 1); }
    if (fails) { std::printf("0094e7b0_TEST FAIL\n"); return 1; }
    std::printf("0094e7b0_TEST PASS\n");
    return 0;
}
