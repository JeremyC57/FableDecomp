// Behaviour test for 0x00BF6490 (CLandscapeBackgroundTreeNode::TestIfLoadTaskCanBeAborted).
#include <cstdio>
// Host model of the retail landscape background tree: a node owns a sub node that is a leaf patch (LODBand)
// or a split branch holding two child nodes; the node keeps an optional async load task.
struct Node;
struct Task { void* owner; unsigned char lod; bool isPatch; };
struct SubNode { bool patch; unsigned char LODBand; Node* left; Node* right; bool IsPatch() { return patch; } };
struct Node {
    Task* AsyncLoadTask; Node* Parent; SubNode* Sub; unsigned char FirstNonSplitBand;
    bool VisibleLastFrame, VisibleLastFrameHasBeenUpdated, TesselationValid;
    Node() : AsyncLoadTask(0), Parent(0), Sub(0), FirstNonSplitBand(0), VisibleLastFrame(false), VisibleLastFrameHasBeenUpdated(false), TesselationValid(true) {}
    SubNode* GetPatch() { if (Sub && Sub->IsPatch()) return Sub; return 0; }
    SubNode* GetSplitBranch() { if (Sub && !Sub->IsPatch()) return Sub; return 0; }
    bool Test(long requiredBand)
    {
        long currentBand = 0;
        SubNode* patch = GetPatch();
        if (patch) currentBand = patch->LODBand;
        else if (!GetSplitBranch()) return false;
        if (requiredBand < FirstNonSplitBand) requiredBand = 0;
        Task* task = AsyncLoadTask;
        long loadingBand = 0;
        if (task->isPatch) loadingBand = task->lod;
        bool abort = false;
        if (requiredBand == currentBand) return true;
        if (requiredBand > currentBand && loadingBand < currentBand) abort = true;
        if (requiredBand < currentBand && loadingBand > currentBand) abort = true;
        return abort;
    }
};
int main()
{
    Node n; SubNode p; p.patch = true; p.LODBand = 3; Task t; t.isPatch = true; t.lod = 1;
    n.AsyncLoadTask = &t; n.FirstNonSplitBand = 2;
    int ok = 1;
    ok &= !n.Test(5);                       // no sub node
    n.Sub = &p;
    ok &= n.Test(3);                        // required == current
    ok &= n.Test(5);                        // required above current, loading below
    t.lod = 4; ok &= !n.Test(5);
    ok &= n.Test(1);             // 1 < FirstNonSplitBand -> 0; 0 < 3 and loading 4 > 3 -> abort
    t.lod = 2; ok &= !n.Test(2);
    t.isPatch = false; ok &= n.Test(5);     // loading 0 < 3
    if (!ok) { std::printf("00bf6490_TEST FAIL\n"); return 1; }
    std::printf("00bf6490_TEST PASS\n");
    return 0;
}