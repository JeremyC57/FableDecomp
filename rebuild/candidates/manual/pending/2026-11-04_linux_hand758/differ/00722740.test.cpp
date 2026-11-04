// Behaviour test for CTCSmokeGenerator::FrameUpdate (0x00722740), g++ model: with positions created, each position lacking a smoke thing gets a new emitter there; otherwise the positions are created.
#include <cstdio>
#include <vector>
struct Pos { float x; int smoke; };
static void FrameUpdate(bool& created, std::vector<Pos>& pos, long emitter, int& nextThing, int& cameraReads)
{
    if (created) {
        for (size_t i = 0; i < pos.size(); ++i) {
            ++cameraReads;
            if (!pos[i].smoke) pos[i].smoke = (int)emitter * 100 + ++nextThing;
        }
    } else { created = true; pos.resize(2); pos[0].x = 1; pos[0].smoke = 0; pos[1].x = 2; pos[1].smoke = 0; }
}
int main()
{
    bool created = false; std::vector<Pos> pos; int next = 0, cam = 0;
    FrameUpdate(created, pos, 7, next, cam);
    int ok = created && pos.size() == 2 && pos[0].smoke == 0 && cam == 0;
    pos[1].smoke = 55;
    FrameUpdate(created, pos, 7, next, cam);
    ok = ok && pos[0].smoke == 701 && pos[1].smoke == 55 && cam == 2;
    FrameUpdate(created, pos, 7, next, cam); ok = ok && next == 1 && cam == 4;
    std::printf(ok ? "00722740_TEST PASS\n" : "00722740_TEST FAIL\n");
    return ok ? 0 : 1;
}
