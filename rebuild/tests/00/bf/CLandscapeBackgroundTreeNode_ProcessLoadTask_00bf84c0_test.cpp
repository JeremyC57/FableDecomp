#include <cstdio>
// Behaviour model of CLandscapeBackgroundTreeNode::ProcessLoadTask (0x00BF84C0): status 1 loaded, 2 still loading, 3 failed.
struct Node {
    double now; bool canAbort; bool blockLoaded; bool isPatch; bool allocOk; bool patchLoadOk; int branchStatus;
    bool hasTask; bool taskHasTree; int subNode; int loadedCalls; int freedTasks;
};
static int Process(Node& n, double deadline)
{
    if (n.now > deadline) return 2;
    if (n.canAbort) { if (n.hasTask) ++n.freedTasks; n.hasTask = false; return 1; }
    if (!n.taskHasTree) {
        if (!n.blockLoaded) return 2;
        if (n.isPatch) {
            if (!n.allocOk) { ++n.freedTasks; n.hasTask = false; return 3; }
            if (!n.patchLoadOk) { ++n.freedTasks; n.hasTask = false; return 3; }
            n.subNode = 1; ++n.freedTasks; n.hasTask = false; ++n.loadedCalls; return 1;
        }
        if (!n.allocOk) return 3;
        n.taskHasTree = true;
    }
    int status = n.branchStatus;
    if (status == 1) { n.subNode = 2; ++n.freedTasks; n.hasTask = false; ++n.loadedCalls; return 1; }
    if (status == 3) { ++n.freedTasks; n.hasTask = false; return 3; }
    return status;
}
int main()
{
    Node base = { 1.0, false, true, true, true, true, 2, true, false, 0, 0, 0 };
    Node a = base; int ok = Process(a, 0.5) == 2 && a.hasTask;
    Node b = base; b.canAbort = true; ok = ok && Process(b, 2.0) == 1 && !b.hasTask && b.freedTasks == 1;
    Node c = base; c.blockLoaded = false; ok = ok && Process(c, 2.0) == 2;
    Node d = base; ok = ok && Process(d, 2.0) == 1 && d.subNode == 1 && d.loadedCalls == 1;
    Node e = base; e.patchLoadOk = false; ok = ok && Process(e, 2.0) == 3 && e.subNode == 0;
    Node f = base; f.isPatch = false; ok = ok && Process(f, 2.0) == 2 && f.taskHasTree && f.hasTask;
    f.branchStatus = 1; ok = ok && Process(f, 2.0) == 1 && f.subNode == 2;
    Node g = base; g.isPatch = false; g.allocOk = false; ok = ok && Process(g, 2.0) == 3 && g.hasTask;
    if (!ok) { std::printf("00bf84c0_TEST FAIL\n"); return 1; }
    std::printf("00bf84c0_TEST PASS\n");
    return 0;
}