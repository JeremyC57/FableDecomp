#include <cstdio>
// Behaviour model of CLandscapeBackgroundTreeNode::ProcessBackgroundLoading (0x00BF8030): the stale-task drop rule and the
// sub-node creation sequence.
struct Node {
    int firstNonSplit; int remap[8]; bool hasTask; bool taskIsPatch; int taskLod; bool wasUpdated;
    int subNode; bool blockLoaded; bool startOk; int started; int childStatus;
};
static bool DropTask(const Node& n, int band, bool visible)
{
    if (visible) {
        if (band >= n.firstNonSplit && n.firstNonSplit < 8) return n.hasTask && n.taskIsPatch && n.remap[band] < n.taskLod;
        if (band < n.firstNonSplit) return n.hasTask && n.taskIsPatch;
        return false;
    }
    if (n.wasUpdated) return false;
    return band >= n.firstNonSplit && n.firstNonSplit < 8 && n.hasTask && (!n.taskIsPatch || n.remap[band] > n.taskLod);
}
static int Process(Node& n, int band, bool visible)
{
    if (DropTask(n, band, visible)) n.hasTask = false;
    if (n.subNode == 0) {
        if (!n.hasTask) {
            if (!n.startOk) return 3;
            n.hasTask = true; n.taskIsPatch = band >= n.firstNonSplit && n.firstNonSplit != 8; ++n.started;
        }
        if (!n.blockLoaded) return 2;
        n.subNode = n.taskIsPatch ? 1 : 2; n.hasTask = false;
        if (n.subNode == 1) return 1;
    }
    if (n.subNode == 1) return 1;
    return n.childStatus;
}
int main()
{
    Node base = { 3, { 0, 1, 2, 3, 4, 5, 6, 7 }, false, false, 0, false, 0, true, true, 0, 2 };
    Node a = base; int ok = Process(a, 5, true) == 1 && a.subNode == 1 && a.started == 1;
    Node b = base; ok = ok && Process(b, 1, true) == 2 && b.subNode == 2;
    Node c = base; c.blockLoaded = false; ok = ok && Process(c, 5, true) == 2 && c.hasTask;
    Node d = base; d.startOk = false; ok = ok && Process(d, 5, true) == 3;
    Node e = base; e.hasTask = true; e.taskIsPatch = true; e.taskLod = 6; e.subNode = 2; ok = ok && DropTask(e, 4, true) && !DropTask(e, 7, true);
    Node f = base; f.hasTask = true; f.taskIsPatch = false; ok = ok && DropTask(f, 4, false) && !DropTask(f, 1, false);
    f.wasUpdated = true; ok = ok && !DropTask(f, 4, false);
    if (!ok) { std::printf("00bf8030_TEST FAIL\n"); return 1; }
    std::printf("00bf8030_TEST PASS\n");
    return 0;
}