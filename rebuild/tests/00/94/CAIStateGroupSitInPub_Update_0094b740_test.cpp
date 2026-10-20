// behaviour test for CAIStateGroup_SitInPub::Update (0x0094B740): logic copy over stubs
#include <cstdio>
static unsigned long ror13(unsigned long v) { return (v >> 13) | (v << 19); }
struct Group {
    bool sat; int idleLeft; long frame; unsigned long seed; long interval;
    bool buildingAlive, buildingHasPub; int removed, terminated, rolls; long nextFidget;
    long DoNothing() { frame += 10; return idleLeft-- > 0 ? 4 : 2; }
    void Update()
    {
        if (sat) {
            nextFidget = frame;
            if (DoNothing() == 4) {
                do {
                    long now = frame;
                    if (now > nextFidget) {
                        unsigned long next = seed * 0x24A1U + 0x24DFU;
                        unsigned long rotated = ror13(next);
                        seed = rotated;
                        unsigned long r = interval ? rotated % interval : 0;
                        nextFidget = interval / 2 + r + now;
                        rolls++;
                    }
                } while (DoNothing() == 4);
            }
        }
        if (buildingAlive && buildingHasPub) removed++;
        terminated++;
    }
};
int main()
{
    Group g = { true, 5, 100, 1234, 30, true, true, 0, 0, 0, 0 };
    g.Update();
    bool ok = g.rolls >= 1 && g.removed == 1 && g.terminated == 1 && g.nextFidget >= 100 + 15;
    Group h = { false, 5, 0, 1, 0, true, false, 0, 0, 0, 0 };
    h.Update();
    if (h.rolls != 0 || h.removed != 0 || h.terminated != 1) ok = false;
    Group k = { true, 3, 0, 7, 0, false, true, 0, 0, 0, 0 };
    k.Update();
    if (k.removed != 0 || k.rolls < 1) ok = false;          // zero interval: r = 0, no division
    if (!ok) { std::printf("0094b740_TEST FAIL\n"); return 1; }
    std::printf("0094b740_TEST PASS\n");
    return 0;
}