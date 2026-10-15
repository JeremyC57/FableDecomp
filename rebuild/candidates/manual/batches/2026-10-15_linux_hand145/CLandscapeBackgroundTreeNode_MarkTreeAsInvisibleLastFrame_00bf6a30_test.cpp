// Behaviour test for 0x00BF6A30 (CLandscapeBackgroundTreeNode::MarkTreeAsInvisibleLastFrame).
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
    void Mark()
    {
        if (VisibleLastFrame) {
            VisibleLastFrame = false; VisibleLastFrameHasBeenUpdated = true;
            if (GetSplitBranch()) { GetSplitBranch()->left->Mark(); GetSplitBranch()->right->Mark(); }
        }
    }
};
int main()
{
    Node root, a, b, c; SubNode split; split.patch = false; split.left = &a; split.right = &b;
    SubNode split2; split2.patch = false; split2.left = &c; split2.right = &c;
    root.Sub = &split; root.VisibleLastFrame = true; a.VisibleLastFrame = true; b.VisibleLastFrame = false; a.Sub = &split2; c.VisibleLastFrame = true;
    root.Mark();
    int ok = !root.VisibleLastFrame && root.VisibleLastFrameHasBeenUpdated && !a.VisibleLastFrame && a.VisibleLastFrameHasBeenUpdated
          && !b.VisibleLastFrameHasBeenUpdated && !c.VisibleLastFrame;
    if (!ok) { std::printf("00bf6a30_TEST FAIL\n"); return 1; }
    std::printf("00bf6a30_TEST PASS\n");
    return 0;
}
