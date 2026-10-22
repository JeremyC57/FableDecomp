// Behaviour test for CDF_Fireheart::Main (0x00D3A1A0): the first run creates the demon door where the placeholder
// stands (zero position/angle without one), makes both immune and persistent, removes the placeholder and sets the
// parent flag; every run marks the door, waits until the hero uses the speaker (aborting on interrupt), then plays the
// cutscene and removes the speaker.
#include <cstdio>
#include <cstring>

struct Vec { float x, y, z; };
struct World {
    bool hasPlaceholder; Vec placeholderPos; float placeholderAngle;
    bool doorCreated; Vec doorPos; float doorAngle; bool placeholderRemoved;
    int marked; int framesUntilUse; int interruptAfter; int frames;
    const char* cutscene; bool speakerRemoved;
};

static bool Interrupted(World* w) { return w->interruptAfter >= 0 && w->frames >= w->interruptAfter; }

static void Main(World* w)
{
    ++w->frames;
    if (Interrupted(w)) return;
    if (!w->doorCreated) {
        Vec zero = { 0, 0, 0 };
        Vec pos = w->hasPlaceholder ? w->placeholderPos : zero;
        float angle = w->hasPlaceholder ? w->placeholderAngle : 0.0f;
        w->doorPos = pos; w->doorAngle = angle;
        w->placeholderRemoved = true;
        w->doorCreated = true;
    }
    ++w->marked;
    while (w->framesUntilUse > 0) {
        --w->framesUntilUse;
        ++w->frames;
        if (Interrupted(w)) return;
    }
    w->cutscene = "CS_FIREHEART_DEMONDOOR";
    w->speakerRemoved = true;
}

int main()
{
    World w;
    std::memset(&w, 0, sizeof(w));
    w.hasPlaceholder = true; w.placeholderPos.x = 3; w.placeholderPos.y = 4; w.placeholderAngle = 1.5f;
    w.framesUntilUse = 2; w.interruptAfter = -1;
    Main(&w);
    if (!w.doorCreated || w.doorPos.x != 3 || w.doorPos.y != 4 || w.doorAngle != 1.5f || !w.placeholderRemoved
        || w.marked != 1 || !w.speakerRemoved || std::strcmp(w.cutscene, "CS_FIREHEART_DEMONDOOR") != 0) {
        std::printf("00d3a1a0_TEST FAIL\n"); return 1;
    }
    World v;
    std::memset(&v, 0, sizeof(v));
    v.framesUntilUse = 5; v.interruptAfter = 3;
    Main(&v);
    if (!v.doorCreated || v.doorPos.x != 0 || v.doorAngle != 0.0f || v.speakerRemoved || v.cutscene != 0) {
        std::printf("00d3a1a0_TEST FAIL\n"); return 1;
    }
    std::printf("00d3a1a0_TEST PASS\n");
    return 0;
}
