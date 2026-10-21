// Behaviour test for CAIStateGroup_CreatureUseCoverBase::IsRunnableLeaf (0x00970F30): after a failed search,
// stay not-runnable while within 15 units and under 8 seconds; otherwise gather cover, recording failure when none.
#include <cstdio>
struct V { float x, y, z; };
struct Leaf {
    int failed; int failedTime; V failedAt; V pos; int frame; int fps; int nThings; int covers;
    bool IsRunnableLeaf()
    {
        if (failed) {
            float dx = failedAt.x - pos.x, dy = failedAt.y - pos.y;
            int seconds = (frame - failedTime) / fps;
            if (dx * dx + dy * dy < 225.0f && seconds < 8) return false;
            failed = 0;
        }
        if (nThings != 0) { covers += nThings; return true; }
        failedAt = pos; failed = 1; return false;
    }
};
int main()
{
    int ok = 1;
    Leaf l; l.failed = 0; l.failedTime = 0; l.fps = 30; l.frame = 100; l.covers = 0;
    l.pos.x = 0; l.pos.y = 0; l.pos.z = 0; l.failedAt = l.pos; l.nThings = 0;
    if (l.IsRunnableLeaf() || !l.failed) ok = 0;                 // no cover: record failure
    l.failedTime = 100; l.frame = 130; l.nThings = 2;
    if (l.IsRunnableLeaf() || !l.failed) ok = 0;                 // within wait window
    l.pos.x = 20.0f;
    if (!l.IsRunnableLeaf() || l.failed || l.covers != 2) ok = 0;  // moved far: retry and gather
    l.failed = 1; l.failedAt = l.pos; l.failedTime = 0; l.frame = 300;
    if (!l.IsRunnableLeaf() || l.covers != 4) ok = 0;            // waited 10 seconds
    std::printf(ok ? "00970f30_TEST PASS\n" : "00970f30_TEST FAIL\n");
    return ok ? 0 : 1;
}