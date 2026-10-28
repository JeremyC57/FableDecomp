// Behaviour test for 0x00A15140 CNavigatorManager::GetNextNeighbour: the edge walk round a node on a model grid --
// the first step (no previous) goes to the smallest cell above the top-left corner, steps along the edges, corner
// jumps, the blocked-node fallback (flag set), and the null return when the walk reaches the start position.
#include <stdio.h>
#include <math.h>

struct V2 { float X, Y; V2() { } V2(float x, float y) : X(x), Y(y) { } V2& operator+=(const V2& v) { X += v.X; Y += v.Y; return *this; } };
struct Node { V2 Position; unsigned char NavLayer; float Size; float GetSize() const { return Size; } };

static float g_Smallest = 1.0f;
static Node g_Blocked;
static Node g_Found;
static bool g_HasNode = true;
static V2 g_LastBlocked;

static const Node* GetNodeAt(const V2& p, unsigned char) { if (!g_HasNode) return 0; g_Found.Position = p; g_Found.Size = 1.0f; return &g_Found; }
static void SetBlockedNode(float x, float y, long, long) { g_LastBlocked = V2(x, y); g_Blocked.Position = g_LastBlocked; }

static const Node* GetNextNeighbour(const Node& node, const Node* previous, const V2& position, bool* blocked)
{
    float halfSize = node.GetSize() * 0.5f;
    float halfSmallest = g_Smallest * 0.5f;
    V2 nodePos = node.Position;
    V2 testPos;
    if (previous == 0) {
        testPos = nodePos;
        testPos.X += halfSmallest - halfSize;
        testPos.Y = testPos.Y + halfSmallest + halfSize;
    } else {
        testPos = previous->Position;
        float prevHalf = previous->GetSize() * 0.5f;
        if (testPos.Y - prevHalf >= nodePos.Y + halfSize) {
            if (prevHalf + testPos.X > nodePos.X + halfSize) {
                testPos.X = nodePos.X + halfSmallest + halfSize;
                testPos.Y = nodePos.Y + halfSize - halfSmallest;
            } else
                testPos += V2(prevHalf + halfSmallest, halfSmallest - prevHalf);
        } else if (testPos.X - prevHalf >= nodePos.X + halfSize) {
            if (testPos.Y - prevHalf < nodePos.Y - halfSize) {
                testPos.X = nodePos.X + halfSize - halfSmallest;
                testPos.Y = nodePos.Y - (halfSmallest + halfSize);
            } else
                testPos += V2(halfSmallest - prevHalf, -(prevHalf + halfSmallest));
        } else if (prevHalf + testPos.Y <= nodePos.Y - halfSize) {
            if (testPos.X - prevHalf < nodePos.X - halfSize) {
                testPos.X = nodePos.X - (halfSmallest + halfSize);
                testPos.Y = nodePos.Y + halfSmallest - halfSize;
            } else
                testPos += V2(-(prevHalf + halfSmallest), prevHalf - halfSmallest);
        } else if (prevHalf + testPos.X <= nodePos.X - halfSize) {
            if (prevHalf + testPos.Y > nodePos.Y + halfSize) {
                testPos.X = nodePos.X + halfSmallest - halfSize;
                testPos.Y = nodePos.Y + halfSmallest + halfSize;
            } else
                testPos += V2(prevHalf - halfSmallest, prevHalf + halfSmallest);
        }
    }
    const Node* next = GetNodeAt(testPos, node.NavLayer);
    if (next == 0) {
        next = &g_Blocked;
        SetBlockedNode(testPos.X, testPos.Y, node.NavLayer, 6);
        if (blocked)
            *blocked = true;
    } else if (blocked)
        *blocked = false;
    if (previous && fabs(next->Position.X - position.X) <= 0.0001f && fabs(next->Position.Y - position.Y) <= 0.0001f)
        return 0;
    return next;
}

static int fail(int line) { printf("00a15140_TEST FAIL (line %d)\n", line); return 1; }
static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }

int main()
{
    Node node; node.Position = V2(10.0f, 10.0f); node.Size = 4.0f; node.NavLayer = 0;   // edges 8..12
    bool blocked = true;
    const Node* n = GetNextNeighbour(node, 0, V2(0, 0), &blocked);
    if (!n || blocked || !Near(n->Position.X, 8.5f) || !Near(n->Position.Y, 12.5f)) return fail(__LINE__);
    Node start = *n;
    // walk the whole perimeter: 4 edges of 4 cells + 4 corners = 20 steps back to the start
    Node prev = start;
    int steps = 0;
    for (;;) {
        const Node* next = GetNextNeighbour(node, &prev, start.Position, &blocked);
        ++steps;
        if (!next) break;
        prev = *next;
        if (steps > 40) return fail(__LINE__);
    }
    if (steps != 20) return fail(__LINE__);
    // above the top edge, moving right
    prev.Position = V2(9.5f, 12.5f); prev.Size = 1.0f;
    n = GetNextNeighbour(node, &prev, V2(0, 0), &blocked);
    if (!Near(n->Position.X, 10.5f) || !Near(n->Position.Y, 12.5f)) return fail(__LINE__);
    // past the top-right corner: jump to the cell right of the top edge
    prev.Position = V2(12.5f, 12.5f);
    n = GetNextNeighbour(node, &prev, V2(0, 0), &blocked);
    if (!Near(n->Position.X, 12.5f) || !Near(n->Position.Y, 11.5f)) return fail(__LINE__);
    // no node there: the blocked node at the probe position, flag set
    g_HasNode = false; blocked = false;
    n = GetNextNeighbour(node, 0, V2(0, 0), &blocked);
    if (n != &g_Blocked || !blocked || !Near(g_LastBlocked.X, 8.5f) || !Near(g_LastBlocked.Y, 12.5f)) return fail(__LINE__);
    g_HasNode = true;
    n = GetNextNeighbour(node, 0, V2(0, 0), 0);
    if (!n) return fail(__LINE__);
    printf("00a15140_TEST PASS\n");
    return 0;
}