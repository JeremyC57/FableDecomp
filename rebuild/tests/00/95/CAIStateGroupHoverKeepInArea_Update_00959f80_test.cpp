// Behaviour test for CAIStateGroup_HoverKeepInArea::Update (0x00959F80): model copy with mocked callees.
#include <cstdio>
#include <cmath>
struct V { float X, Y, Z; V() {} V(float x, float y, float z) : X(x), Y(y), Z(z) {} };
static unsigned long ror13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long GFRandom(unsigned long range, unsigned long* seed) { *seed = *seed * 0x24A1 + 0x24DF; *seed = ror13(*seed); return *seed % range; }
struct Mock {
    unsigned long seed; bool navOk; V wander; V creaturePos[8]; int posIdx; int lookRet; int moveRets[8]; int moveIdx;
    bool terminated; int movementSeen; V lookSeen;
};
static void Update(Mock& m, const V& StartPos)
{
    unsigned long* seed = &m.seed;
    if (!m.navOk) return;
    V target = m.wander;
    float angle = (float)GFRandom(100, seed) * 0.003f + 0.1f;
    if (GFRandom(2, seed) == 0) angle *= -1.0f;
    float dist = (float)GFRandom(100, seed) * 0.02f + 3.0f;
    V here = m.creaturePos[0];
    V dir(target.X - here.X, target.Y - here.Y, 0.0f);
    float len = std::sqrt(dir.X * dir.X + dir.Y * dir.Y);
    if (len > 0) { dir.X /= len; dir.Y /= len; }
    V perp(dir.Y, -dir.X, 0.0f);
    double a = angle * 6.283185482025146484375;
    float s = (float)std::sin(a), c = (float)std::cos(a);
    V look(target.X + perp.X * s * dist + dir.X * c * dist, target.Y + perp.Y * s * dist + dir.Y * c * dist, target.Z);
    m.lookSeen = look;
    if (m.lookRet != 2) { m.terminated = true; return; }
    int movement = (int)GFRandom(3, seed) + 8;
    m.movementSeen = movement;
    int result; float dx, dy;
    do {
        V p = m.creaturePos[m.posIdx++];
        dx = p.X - StartPos.X; dy = p.Y - StartPos.Y;
        result = m.moveRets[m.moveIdx++];
    } while (result == 4 && (float)std::sqrt(dy * dy + dx * dx) >= 20.0f);
    m.terminated = true;
}
int main()
{
    V start(0, 0, 0);
    Mock m = {};
    m.seed = 12345; m.navOk = true; m.wander = V(10, 0, 0); m.lookRet = 2;
    m.creaturePos[0] = V(0, 0, 0); m.creaturePos[1] = V(30, 0, 0); m.creaturePos[2] = V(25, 0, 0); m.creaturePos[3] = V(5, 0, 0);
    m.moveRets[0] = 4; m.moveRets[1] = 4; m.moveRets[2] = 4; m.moveRets[3] = 4;
    Update(m, start);
    // loop: pos0 (0,0) dist 0 <20 -> stop after first move
    bool ok = m.terminated && m.moveIdx == 1 && m.movementSeen >= 8 && m.movementSeen <= 10;
    // look point lies 3..5 units from the target
    float lx = m.lookSeen.X - 10, ly = m.lookSeen.Y;
    float ld = std::sqrt(lx * lx + ly * ly);
    ok = ok && ld >= 2.99f && ld <= 5.0f;
    Mock m2 = m; m2.posIdx = 1; m2.moveIdx = 0; m2.seed = 12345; m2.terminated = false;
    Update(m2, start); // pos (30,0),(25,0) far: continue; (5,0) near: stop -> 3 moves
    ok = ok && m2.moveIdx == 3;
    Mock m3 = m; m3.seed = 1; m3.lookRet = 0; m3.terminated = false; m3.moveIdx = 0;
    Update(m3, start);
    ok = ok && m3.terminated && m3.moveIdx == 0;
    Mock m4 = m; m4.navOk = false; m4.terminated = false;
    Update(m4, start);
    ok = ok && !m4.terminated;
    if (!ok) { std::printf("00959f80_TEST FAIL\n"); return 1; }
    std::printf("00959f80_TEST PASS\n");
    return 0;
}