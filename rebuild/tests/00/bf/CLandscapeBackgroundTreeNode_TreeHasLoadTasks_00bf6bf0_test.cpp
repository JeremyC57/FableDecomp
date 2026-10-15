// Behaviour test for 0x00BF6BF0 (CLandscapeBackgroundTreeNode::TreeHasLoadTasks).
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
    bool Has()
    {
        if (AsyncLoadTask) return true;
        if (GetSplitBranch()) return GetSplitBranch()->left->Has() || GetSplitBranch()->right->Has();
        return false;
    }
};
int main()
{
    Node root, a, b; SubNode s; s.patch = false; s.left = &a; s.right = &b; Task t;
    int ok = !root.Has();
    root.Sub = &s; ok &= !root.Has();
    b.AsyncLoadTask = &t; ok &= root.Has();
    SubNode p; p.patch = true; root.Sub = &p; ok &= !root.Has();
    root.AsyncLoadTask = &t; ok &= root.Has();
    if (!ok) { std::printf("00bf6bf0_TEST FAIL\n"); return 1; }
    std::printf("00bf6bf0_TEST PASS\n");
    return 0;
}