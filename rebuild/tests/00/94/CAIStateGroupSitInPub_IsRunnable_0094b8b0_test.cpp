// behaviour test for CAIStateGroup_SitInPub::IsRunnable (0x0094B8B0): logic copy over stubs
#include <cstdio>
struct Thing { bool hasPub; bool acceptsUser; int users; };
struct Village { Thing* nearestPub; };
struct Group {
    bool villager; Village* village; Thing* building; Thing* seat; Thing* taskSeat; Thing* randomSeat; bool seatValid;
    bool IsRunnable()
    {
        if (villager) {
            if (village) {
                Thing* pub = village->nearestPub;
                if (pub && pub->hasPub) {
                    if (pub->acceptsUser) {
                        pub->users++;
                        building = pub;
                        Thing* s = taskSeat;
                        if (s) seat = s; else seat = randomSeat;
                    }
                }
            }
        }
        return seat != 0 && seatValid;
    }
};
int main()
{
    Thing pub = { true, true, 0 }, chairA = { false, false, 0 }, chairB = { false, false, 0 };
    Village v = { &pub };
    Group g = { true, &v, 0, 0, 0, &chairB, true };
    bool ok = g.IsRunnable() && g.building == &pub && g.seat == &chairB && pub.users == 1;
    g.taskSeat = &chairA;
    if (!g.IsRunnable() || g.seat != &chairA) ok = false;
    Group h = { false, &v, 0, 0, &chairA, &chairB, true };
    if (h.IsRunnable() || h.building != 0) ok = false;                 // not a villager, no seat yet
    pub.acceptsUser = false;
    Group k = { true, &v, 0, 0, &chairA, &chairB, true };
    if (k.IsRunnable() || k.building != 0) ok = false;                 // pub full
    if (!ok) { std::printf("0094b8b0_TEST FAIL\n"); return 1; }
    std::printf("0094b8b0_TEST PASS\n");
    return 0;
}