// Behaviour test for 0x00BF6AC0 (CLandscapeBackgroundTreeNode::UpdateParentChainVisibilityFlag).
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
    void Update()
    {
        bool visible = GetSplitBranch()->left->VisibleLastFrame || GetSplitBranch()->right->VisibleLastFrame;
        if (VisibleLastFrame != visible) {
            VisibleLastFrame = visible; VisibleLastFrameHasBeenUpdated = true;
            if (Parent) Parent->Update();
        }
    }
};
int main()
{
    Node top, mid, a, b, c; SubNode s1; s1.patch = false; s1.left = &mid; s1.right = &c;
    SubNode s2; s2.patch = false; s2.left = &a; s2.right = &b;
    top.Sub = &s1; mid.Sub = &s2; mid.Parent = &top;
    b.VisibleLastFrame = true;
    mid.Update();
    int ok = mid.VisibleLastFrame && mid.VisibleLastFrameHasBeenUpdated && top.VisibleLastFrame && top.VisibleLastFrameHasBeenUpdated;
    b.VisibleLastFrame = false; top.VisibleLastFrameHasBeenUpdated = false; c.VisibleLastFrame = true;
    mid.Update();
    ok &= !mid.VisibleLastFrame && top.VisibleLastFrame && !top.VisibleLastFrameHasBeenUpdated;
    if (!ok) { std::printf("00bf6ac0_TEST FAIL\n"); return 1; }
    std::printf("00bf6ac0_TEST PASS\n");
    return 0;
}
