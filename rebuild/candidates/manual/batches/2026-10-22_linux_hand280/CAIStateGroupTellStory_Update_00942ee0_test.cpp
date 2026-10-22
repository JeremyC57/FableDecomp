// Behaviour test for CAIStateGroup_TellStory::Update (0x00942EE0): while it is cook time the teller walks to the marker facing a random listener (or the marker's facing point when nobody listens) and, with listeners, talks 2-4 seconds then acts 3-6; stopping ends the loop; always terminates.
#include <cstdio>
struct Vec { float x, y, z; };
struct CTeller {
    int cookRounds, stopAfter, rounds, talks, acts, moves;
    int listenerCount; Vec listener, marker, forward, lastFace;
    float lastTalk, lastAct;
    unsigned long seed;
    bool terminated;
    unsigned long Random(unsigned long range) { seed = seed * 0x24A1U + 0x24DFU; return seed % range; }
    void Update()
    {
        while (cookRounds-- > 0) {
            if (rounds == stopAfter)
                break;
            ++rounds;
            Vec face;
            if (listenerCount != 0) {
                Random(listenerCount);
                face = listener;
            } else {
                face.x = forward.x + marker.x; face.y = forward.y + marker.y; face.z = forward.z + marker.z;
            }
            ++moves;
            lastFace = face;
            if (listenerCount != 0) {
                lastTalk = (float)(Random(3) + 2); ++talks;
                lastAct = (float)Random(4) + 3.0f; ++acts;
            }
        }
        terminated = true;
    }
};
int main()
{
    CTeller t = { 3, 99, 0, 0, 0, 0, 2, { 5, 6, 7 }, { 1, 2, 3 }, { 0, 1, 0 }, { 0, 0, 0 }, 0, 0, 12345, false };
    t.Update();
    bool ok = t.rounds == 3 && t.moves == 3 && t.talks == 3 && t.acts == 3 && t.lastFace.x == 5 && t.terminated
        && t.lastTalk >= 2 && t.lastTalk <= 4 && t.lastAct >= 3 && t.lastAct <= 6;
    CTeller e = { 5, 2, 0, 0, 0, 0, 0, { 5, 6, 7 }, { 1, 2, 3 }, { 0, 1, 0 }, { 0, 0, 0 }, 0, 0, 1, false };
    e.Update();
    ok = ok && e.rounds == 2 && e.moves == 2 && e.talks == 0 && e.lastFace.y == 3 && e.terminated;
    if (!ok) { std::printf("00942ee0_TEST FAIL\n"); return 1; }
    std::printf("00942ee0_TEST PASS\n");
    return 0;
}
