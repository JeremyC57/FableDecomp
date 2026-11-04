// Behaviour test for 0x00A1ABF0 CNavigatorAStar::IsNewDestinationGoingToCauseRestart(const C3DVector&, float) const: a model of
// the row. Idle/interrupted or off-map destinations restart; while routing a different node restarts unless it neighbours the
// node asked for (blocked nodes compare the nearest navigable node with the goal node); otherwise the planar distance from the
// goal position is tested against (reach radius + radius) squared.
#include <cstdio>
struct Node { int id; bool Blocked; int neighbour; bool IsNeighbourOf(const Node* o) const { return o && o->id == neighbour; } };
struct Nav {
    bool Interrupted, OnRoute, RouteEmpty, HeadingForActionPoint;
    const Node* nodeAtDest; const Node* NodeAskedToGetTo; const Node* GoalNode; const Node* nearest;
    float GoalX, GoalY, reachRadius;
    static bool IsEqual(const Node* a, const Node* b) { if (a == b) return true; if (!a || !b) return false; return a->id == b->id; }
    bool Test(float dx0, float dy0, float radius) const
    {
        if (!Interrupted && OnRoute) {
            const Node* node = nodeAtDest;
            if (node) {
                if (!RouteEmpty && !HeadingForActionPoint && !IsEqual(node, NodeAskedToGetTo)) {
                    if (!node->Blocked) return node->IsNeighbourOf(NodeAskedToGetTo) ? false : true;
                    return IsEqual(nearest, GoalNode) ? false : true;
                }
                float reach = reachRadius;
                radius += reach;
                radius *= radius;
                if (node == NodeAskedToGetTo || (NodeAskedToGetTo && node->id == NodeAskedToGetTo->id)) {
                    float dx = GoalX - dx0, dy = GoalY - dy0;
                    if (!(dx * dx + dy * dy > radius)) return false;
                }
            }
        }
        return true;
    }
};
int main()
{
    Node a = { 1, false, 2 }, b = { 2, false, 1 }, c = { 3, true, 0 }, g = { 9, false, 0 };
    Nav n = { false, true, true, false, &a, &a, &g, &g, 0.0f, 0.0f, 1.0f };
    int ok = 1;
    ok &= n.Test(1.0f, 1.0f, 1.0f) == false;          // same node, inside (1+1)^2
    ok &= n.Test(3.0f, 0.0f, 1.0f) == true;           // outside the reach circle
    n.Interrupted = true; ok &= n.Test(0.0f, 0.0f, 1.0f) == true; n.Interrupted = false;
    n.RouteEmpty = false; n.nodeAtDest = &b; ok &= n.Test(0.0f, 0.0f, 1.0f) == false;  // neighbour of the asked node
    n.nodeAtDest = &c; ok &= n.Test(0.0f, 0.0f, 1.0f) == false;                        // blocked: nearest is the goal
    n.nearest = &a; ok &= n.Test(0.0f, 0.0f, 1.0f) == true;
    n.nodeAtDest = 0; ok &= n.Test(0.0f, 0.0f, 1.0f) == true;
    std::printf(ok ? "00a1abf0_TEST PASS\n" : "00a1abf0_TEST FAIL\n");
    return ok ? 0 : 1;
}