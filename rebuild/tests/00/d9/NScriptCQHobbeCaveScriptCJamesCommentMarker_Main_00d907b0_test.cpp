#include <cstdio>
// Behaviour model of CJamesCommentMarker::Main (0x00D907B0): per frame, while James is near and the hobbes are not close by,
// once the counter passes the comment delay James comments (marker removed, script ends); a refused comment pulls the
// counter back to one second under the delay; James away (or hobbes close) restarts the counter.
struct World { int frames; bool nearAt[16]; bool hobbes; bool refuse; int removedAt; int comments; };
static int Run(World& w, int delay)
{
    float counter = 0.0f;                         // seconds elapsed (one second per frame here)
    for (int f = 0; f < w.frames; ++f) {
        if (!w.hobbes && w.nearAt[f]) {
            bool commented = !w.hobbes && counter > (float)delay && !w.refuse;
            if (commented) { ++w.comments; w.removedAt = f; return f; }
            if (counter > (float)delay)
                counter = (float)(delay - 1);
        } else {
            counter = 0.0f;
        }
        counter += 1.0f;
    }
    return -1;
}
int main()
{
    World w; w.frames = 16; w.hobbes = false; w.refuse = false; w.removedAt = -1; w.comments = 0;
    for (int i = 0; i < 16; ++i) w.nearAt[i] = true;
    w.nearAt[2] = false;                          // James wanders off on frame 2: counter restarts
    int ok = Run(w, 3) == 6 && w.comments == 1;
    World r = w; r.refuse = true; r.comments = 0;
    ok = ok && Run(r, 3) == -1 && r.comments == 0;
    World h = w; h.hobbes = true; h.comments = 0;
    ok = ok && Run(h, 3) == -1;
    if (!ok) { std::printf("00d907b0_TEST FAIL\n"); return 1; }
    std::printf("00d907b0_TEST PASS\n");
    return 0;
}