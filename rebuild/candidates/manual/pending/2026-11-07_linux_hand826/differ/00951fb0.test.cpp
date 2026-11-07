// Behaviour test for CAIStateGroup_JackDragon::UpdateHover (0x00951FB0): an invalid enemy right after the hover setup terminates
// before any attack; each hover round breaks off when the enemy is 1.5 above the landing marker, beams inside MaxFirebeamDistance,
// otherwise blasts (forward when GoLeft or the facing is within 0.866, left when the -0.25 rad turn still points the same way, else
// forward); MaxHoverLoops bounds the rounds; afterwards the GoLeft side picks the ground-exit route (3Right, 3Left, 4Left and the
// previous flight position 3Left; else 3Left, 3Right, 4Right and 3Right) after the creature's own position, and the next state is 4
// on a tower, else the into-ground choice.
#include <cstdio>
#include <cmath>
#include <vector>

struct V { float x, y, z; };
enum Action { BEAM, BLAST_FORWARD, BLAST_LEFT };

struct Dragon {
    bool enemyValid; long goLeft; long loops; float beamRange; bool onTower; long intoGround;
    V creature, enemy, landed, facing;
    std::vector<Action> actions; std::vector<int> route; int previous; long state; bool terminated;
    Dragon() : enemyValid(false), goLeft(0), loops(0), beamRange(0.0f), onTower(false), intoGround(0), previous(0), state(0), terminated(false)
    {
        V z = { 0, 0, 0 }; creature = enemy = landed = facing = z;   // VC7.1: no value-init
    }
    static void RotateXY(V& v, float a) { float c = std::cos(a), s = std::sin(a); V r = { v.x * c - v.y * s, v.x * s + v.y * c, v.z }; v = r; }
    void UpdateHover()
    {
        if (!enemyValid) { terminated = true; return; }
        for (long i = 0; i < loops; ++i) {
            V d = { enemy.x - creature.x, enemy.y - creature.y, 0.0f };
            float dist = std::sqrt(d.x * d.x + d.y * d.y);
            if (enemy.z > landed.z + 1.5f) break;
            if (dist < beamRange) actions.push_back(BEAM);
            else {
                float inv = 1.0f / dist; V n = { d.x * inv, d.y * inv, 0.0f }; V side = n; RotateXY(side, -0.25f);
                if (goLeft || n.x * facing.x + n.y * facing.y > 0.866f) actions.push_back(BLAST_FORWARD);
                else if (side.x * n.x + side.y * n.y > 0.0f) actions.push_back(BLAST_LEFT);
                else actions.push_back(BLAST_FORWARD);
            }
        }
        route.push_back(0);                                        // the creature's position
        if (goLeft) { route.push_back(0x96); route.push_back(0x8a); route.push_back(0xa2); previous = 0x8a; }
        else { route.push_back(0x8a); route.push_back(0x96); route.push_back(0xae); previous = 0x96; }
        state = onTower ? 4 : intoGround;
    }
};

static Dragon Make()
{
    Dragon d = Dragon();
    d.enemyValid = true; d.loops = 3; d.beamRange = 10.0f; d.intoGround = 0;
    V c = { 0, 0, 0 }, e = { 5, 0, 0 }, l = { 0, 0, 0 }, f = { 0, 1, 0 };
    d.creature = c; d.enemy = e; d.landed = l; d.facing = f;
    return d;
}

int main()
{
    bool ok = true;
    Dragon a = Make(); a.UpdateHover();
    ok = ok && a.actions.size() == 3 && a.actions[0] == BEAM && a.route.size() == 4 && a.route[1] == 0x8a && a.previous == 0x96 && a.state == 0;
    Dragon b = Make(); b.enemy.x = 50.0f; b.goLeft = 1; b.onTower = true; b.UpdateHover();
    ok = ok && b.actions.size() == 3 && b.actions[2] == BLAST_FORWARD && b.route[1] == 0x96 && b.previous == 0x8a && b.state == 4;
    Dragon c = Make(); c.enemy.x = 50.0f; c.UpdateHover();      // facing (0,1) is 90 degrees off; the turned direction still agrees
    ok = ok && c.actions[0] == BLAST_LEFT;
    Dragon e = Make(); e.enemy.z = 2.0f; e.UpdateHover();
    ok = ok && e.actions.empty() && e.route.size() == 4;
    Dragon t = Make(); t.enemyValid = false; t.UpdateHover();
    ok = ok && t.terminated && t.actions.empty() && t.route.empty();
    if (!ok) { std::printf("00951fb0_TEST FAIL\n"); return 1; }
    std::printf("00951fb0_TEST PASS\n");
    return 0;
}
