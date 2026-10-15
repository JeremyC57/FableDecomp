#include <cstdio>
// Behaviour test for CThingTrackNode::PeekTrackStartNode (0x00836DA0): walks previous links to the first IsStart node.
struct CThingTrackNode {
    CThingTrackNode* PreviousNode;
    bool IsStart;
    CThingTrackNode* PeekTrackStartNode()
    {
        CThingTrackNode* node = this;
        while (node) {
            if (node->IsStart)
                return node;
            node = node->PreviousNode;
        }
        return 0;
    }
};
int main()
{
    CThingTrackNode a, b, c;
    a.PreviousNode = 0; a.IsStart = true;
    b.PreviousNode = &a; b.IsStart = false;
    c.PreviousNode = &b; c.IsStart = false;
    if (c.PeekTrackStartNode() != &a || a.PeekTrackStartNode() != &a) { std::printf("00836da0_TEST FAIL\n"); return 1; }
    b.IsStart = true;
    if (c.PeekTrackStartNode() != &b) { std::printf("00836da0_TEST FAIL\n"); return 1; }
    a.IsStart = false; b.IsStart = false;
    if (c.PeekTrackStartNode() != 0) { std::printf("00836da0_TEST FAIL\n"); return 1; }
    std::printf("00836da0_TEST PASS\n");
    return 0;
}
