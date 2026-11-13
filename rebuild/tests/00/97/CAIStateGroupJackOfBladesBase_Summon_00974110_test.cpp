// Behaviour test for 0x00974110 CAIStateGroup_JackOfBladesBase::Summon: a self-contained model of the row (same order of checks,
// position filter, random pick, creature set-up and action choice), driven through the summon loop, the early exits and both actions,
// and checked against the expected call trace and spawn positions. Plain C++98; no row object linked.
#include <cstdio>
#include <cstring>
#include <cmath>

struct V3 { float X, Y, Z; V3() : X(0), Y(0), Z(0) { } V3(float x, float y, float z) : X(x), Y(y), Z(z) { } };
static float Dist(const V3& a, const V3& b) { float x = a.X - b.X, y = a.Y - b.Y, z = a.Z - b.Z; return (float)std::sqrt(x * x + y * y + z * z); }
static unsigned long Ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long Random(unsigned long range, unsigned long* seed) { *seed *= 0x24a1; *seed += 0x24df; *seed = Ror13(*seed); return range ? *seed % range : 0; }

struct Jack {
    // world model
    bool has_solid, def_found, centre_alive, target_alive, creature_ok; int alive_checks_left;
    V3 centre, target, me; V3 candidates[8]; int ncandidates; long creatures[4]; int ncreatures; unsigned long seed;
    int action_result_after; int actions_done;
    // observations
    char trace[512]; V3 spawned[8]; long spawned_def[8]; int nspawned;
    void Log(const char* s) { std::strcat(trace, s); std::strcat(trace, ","); }

    void Summon(bool force_push_close_enemy)
    {
        if (!has_solid) Log("addsolid");
        if (def_found) {
            long count = ncreatures, index = 0;
            long result;
            do {
                if (!(centre_alive && alive_checks_left-- > 0) || !target_alive) { Log("terminate"); return; }
                V3 positions[8]; int n = ncandidates;
                for (int i = 0; i < n; ++i) positions[i] = candidates[i];
                if (n == 0) { Log("nopos"); return; }
                int kept = 0;
                for (int i = 0; i < n; ++i) if (Dist(positions[i], centre) > 5.0f) positions[kept++] = positions[i];
                n = kept;
                if (n == 0) { Log("nofar"); return; }
                V3 pos = positions[Random((unsigned long)n, &seed)];
                long def = creatures[index++];
                if (creature_ok) {
                    spawned[nspawned] = pos; spawned_def[nspawned] = def; ++nspawned;
                    Log("spawn");
                }
                float dx = target.X - me.X, dy = target.Y - me.Y;
                if (force_push_close_enemy && (float)std::sqrt(dx * dx + dy * dy) < 5.0f) Log("push");
                else Log("loop");
                result = (++actions_done >= action_result_after) ? 1 : 2;
            } while (index < count && result != 1);
        }
    }
};

static void Init(Jack& j)
{
    std::memset(&j, 0, sizeof(j));
    j.has_solid = true; j.def_found = true; j.centre_alive = true; j.target_alive = true; j.creature_ok = true; j.alive_checks_left = 100;
    j.centre = V3(0, 0, 0); j.target = V3(3, 0, 0); j.me = V3(0, 0, 0);
    j.candidates[0] = V3(1, 1, 0); j.candidates[1] = V3(6, 0, 0); j.candidates[2] = V3(0, 7, 0); j.candidates[3] = V3(2, 2, 0);
    j.ncandidates = 4; j.creatures[0] = 11; j.creatures[1] = 22; j.creatures[2] = 33; j.ncreatures = 3; j.seed = 5;
    j.action_result_after = 100;
}

int main()
{
    bool ok = true;
    Jack j;
    // 1. three creatures, target far: SUMMON_LOOP each time; only the far candidates are used
    Init(j); j.target = V3(30, 0, 0);
    j.Summon(true);
    ok = ok && std::strcmp(j.trace, "spawn,loop,spawn,loop,spawn,loop,") == 0 && j.nspawned == 3;
    for (int i = 0; i < j.nspawned; ++i) ok = ok && Dist(j.spawned[i], j.centre) > 5.0f && j.spawned_def[i] == 11 * (i + 1);
    // 2. target close and push requested: force push; without the request: loop
    Init(j); j.Summon(true);
    ok = ok && std::strcmp(j.trace, "spawn,push,spawn,push,spawn,push,") == 0;
    Init(j); j.Summon(false);
    ok = ok && std::strcmp(j.trace, "spawn,loop,spawn,loop,spawn,loop,") == 0;
    // 3. action result 1 stops after the first creature
    Init(j); j.action_result_after = 1; j.Summon(false);
    ok = ok && std::strcmp(j.trace, "spawn,loop,") == 0;
    // 4. no solid TC: added first; def missing: nothing else
    Init(j); j.has_solid = false; j.def_found = false; j.Summon(false);
    ok = ok && std::strcmp(j.trace, "addsolid,") == 0;
    // 5. target dead / arena centre gone mid-loop: terminate
    Init(j); j.target_alive = false; j.Summon(false);
    ok = ok && std::strcmp(j.trace, "terminate,") == 0;
    Init(j); j.alive_checks_left = 1; j.Summon(false);
    ok = ok && std::strcmp(j.trace, "spawn,loop,terminate,") == 0;
    // 6. no valid / no far position: return without terminate
    Init(j); j.ncandidates = 0; j.Summon(false);
    ok = ok && std::strcmp(j.trace, "nopos,") == 0;
    Init(j); j.candidates[1] = V3(1, 0, 0); j.candidates[2] = V3(0, 2, 0); j.Summon(false);
    ok = ok && std::strcmp(j.trace, "nofar,") == 0;
    // 7. failed creature creation still plays the action
    Init(j); j.creature_ok = false; j.ncreatures = 1; j.Summon(false);
    ok = ok && std::strcmp(j.trace, "loop,") == 0 && j.nspawned == 0;
    if (!ok) { std::printf("00974110_TEST FAIL\n"); return 1; }
    std::printf("00974110_TEST PASS\n");
    return 0;
}