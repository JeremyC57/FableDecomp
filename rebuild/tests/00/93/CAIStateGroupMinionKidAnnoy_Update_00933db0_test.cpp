// Behaviour test for CAIStateGroup_MinionKidAnnoy::Update (0x00933DB0): while the target lives and is within MAX_RANGE (9) in XY,
// run to it (proximity = the two physics radii); IN_PROGRESS repeats the loop, anything but FINISHED terminates; on arrival, when the
// target still lives, kick it and terminate unless the kick FINISHED. Every exit calls Terminate once.
#include <cstdio>

enum { TERMINATE = 1, FINISHED = 2, FAILED = 3, IN_PROGRESS = 4 };
struct Pos { float X, Y, Z; };
struct Thing { Pos pos; float radius; bool dead; };

static bool SmallerThan2D(const Pos& a, const Pos& b, float compare)
{
    float sqr_dist = (a.X - b.X) * (a.X - b.X) + (a.Y - b.Y) * (a.Y - b.Y);
    return sqr_dist < compare * compare;
}

struct Group {
    Thing* creature; Thing* target;
    const int* move_results; int moves; float last_range;
    const int* kick_results; int kicks;
    int terminated; int kill_at;
    int MoveTo(float range) { last_range = range; if (moves == kill_at) target->dead = true; return move_results[moves++]; }
    int Kick() { return kick_results[kicks++]; }
    void Terminate() { ++terminated; }
    void Update()
    {
        float MAX_RANGE = 9.0f;
        while (target && !target->dead) {
            if (!SmallerThan2D(creature->pos, target->pos, MAX_RANGE))
                break;
            int ret;
            {
                float attack_range = creature->radius + target->radius;
                ret = MoveTo(attack_range);
            }
            if (ret == IN_PROGRESS)
                continue;
            if (ret != FINISHED)
                break;
            if (target && !target->dead) {
                ret = Kick();
                if (ret != FINISHED)
                    break;
            }
        }
        Terminate();
    }
};

static int fail(int n) { std::printf("00933db0_TEST FAIL %d\n", n); return 1; }

static Group make(Thing* c, Thing* t, const int* m, const int* k)
{
    Group g; g.creature = c; g.target = t; g.move_results = m; g.moves = 0; g.last_range = 0; g.kick_results = k; g.kicks = 0;
    g.terminated = 0; g.kill_at = -1; return g;
}

int main()
{
    Thing me = { { 0, 0, 50 }, 0.5f, false };
    Thing kid = { { 3, 4, -20 }, 0.25f, false };                 // 5m away in XY (Z ignored)
    int m1[] = { IN_PROGRESS, FINISHED, FAILED }; int k1[] = { FINISHED, TERMINATE };
    Group g = make(&me, &kid, m1, k1); g.Update();
    if (g.moves != 3 || g.kicks != 1 || g.terminated != 1 || g.last_range != 0.75f) return fail(1);   // in progress, kick, then move fails
    int m2[] = { FAILED }; int k2[] = { FINISHED };
    g = make(&me, &kid, m2, k2); g.Update();
    if (g.moves != 1 || g.kicks != 0 || g.terminated != 1) return fail(2);                          // move failed
    Thing far = { { 6, 7, 0 }, 0.25f, false };                    // 85 > 81
    g = make(&me, &far, m2, k2); g.Update();
    if (g.moves != 0 || g.terminated != 1) return fail(3);
    Thing edge = { { 0, 9, 0 }, 0.25f, false };                   // exactly 9: not smaller
    g = make(&me, &edge, m2, k2); g.Update();
    if (g.moves != 0 || g.terminated != 1) return fail(4);
    Thing dead = { { 1, 1, 0 }, 0.25f, true };
    g = make(&me, &dead, m2, k2); g.Update();
    if (g.moves != 0 || g.terminated != 1) return fail(5);
    g = make(&me, 0, m2, k2); g.Update();
    if (g.moves != 0 || g.terminated != 1) return fail(6);
    int m3[] = { FINISHED, FINISHED, FAILED }; int k3[] = { FINISHED, FAILED };
    g = make(&me, &kid, m3, k3); g.Update();                     // two kicks, the second fails
    if (g.moves != 2 || g.kicks != 2 || g.terminated != 1) return fail(7);
    g = make(&me, &kid, m3, k3); g.kill_at = 0; g.Update();       // the target dies while the kid runs to it: no kick
    if (g.moves != 1 || g.kicks != 0 || g.terminated != 1) return fail(8);
    std::printf("00933db0_TEST PASS\n");
    return 0;
}