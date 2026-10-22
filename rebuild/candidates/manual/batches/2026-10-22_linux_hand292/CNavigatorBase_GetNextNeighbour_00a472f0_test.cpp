// Behaviour test for CNavigatorBase::GetNextNeighbour (0x00A472F0): the clockwise border walk round a node -- the
// top-left start point, the step along / off each edge from the previous neighbour, the blocked-node fallback when
// the manager has no node at the point, and the 0 result once the walk returns to the start position.
#include <cstdio>
#include <cmath>
struct V2 {
    float X, Y;
    V2() {}
    V2(float x, float y) : X(x), Y(y) {}
    V2& operator+=(const V2& v) { X += v.X; Y += v.Y; return *this; }
};
struct Node { V2 Position; float Size; unsigned char Level, Layer; float GetSize() const { return Size; } };
static float GetSmallestNodeSize() { return 0.5f; }
static Node g_Blocked;
static Node g_Hit;
static bool g_ReturnHit;
static V2 g_Asked;
static Node* GetNodeAt(const V2& p, unsigned char) { g_Asked = p; return g_ReturnHit ? &g_Hit : 0; }
static void SetBlockedNode(long layer, long level, V2 p) { g_Blocked.Position = p; g_Blocked.Layer = (unsigned char)layer; g_Blocked.Level = (unsigned char)level; }
static const float kEps = 1e-4f;

static Node* GetNextNeighbour(const Node* node, const Node* prev, const V2& start)
{
    float halfSize = node->GetSize() * 0.5f;
    float step = GetSmallestNodeSize() * 0.5f;
    V2 centre = node->Position;
    V2 pos;
    if (prev == 0) {
        pos = centre;
        pos.X += step - halfSize;
        pos.Y = pos.Y + step + halfSize;
    } else {
        pos = prev->Position;
        float prevHalf = prev->GetSize() * 0.5f;
        if (pos.Y - prevHalf >= centre.Y + halfSize) {
            if (prevHalf + pos.X > centre.X + halfSize) { pos.X = centre.X + step + halfSize; pos.Y = centre.Y + halfSize - step; }
            else pos += V2(prevHalf + step, step - prevHalf);
        } else if (pos.X - prevHalf >= centre.X + halfSize) {
            if (pos.Y - prevHalf < centre.Y - halfSize) { pos.X = centre.X + halfSize - step; pos.Y = centre.Y - (step + halfSize); }
            else pos += V2(step - prevHalf, -(prevHalf + step));
        } else if (prevHalf + pos.Y <= centre.Y - halfSize) {
            if (pos.X - prevHalf < centre.X - halfSize) { pos.X = centre.X - (step + halfSize); pos.Y = centre.Y + step - halfSize; }
            else pos += V2(-(prevHalf + step), prevHalf - step);
        } else if (prevHalf + pos.X <= centre.X - halfSize) {
            if (prevHalf + pos.Y > centre.Y + halfSize) { pos.X = centre.X + step - halfSize; pos.Y = centre.Y + step + halfSize; }
            else pos += V2(prevHalf - step, prevHalf + step);
        }
    }
    Node* found = GetNodeAt(pos, node->Layer);
    if (found == 0) { found = &g_Blocked; SetBlockedNode(node->Layer, 6, pos); }
    if (prev != 0 && std::fabs(found->Position.X - start.X) <= kEps && std::fabs(found->Position.Y - start.Y) <= kEps)
        return 0;
    return found;
}

static bool Near(const V2& a, float x, float y) { return std::fabs(a.X - x) < 1e-5f && std::fabs(a.Y - y) < 1e-5f; }

int main()
{
    int bad = 0;
    Node n; n.Position = V2(0.0f, 0.0f); n.Size = 2.0f; n.Layer = 3; n.Level = 1;   // half 1, step 0.25
    V2 start(-0.75f, 1.25f);
    g_ReturnHit = false;
    Node* r = GetNextNeighbour(&n, 0, start);                   // start point: top-left, just outside the top edge
    if (r != &g_Blocked || !Near(g_Asked, -0.75f, 1.25f) || !Near(g_Blocked.Position, -0.75f, 1.25f) || g_Blocked.Level != 6 || g_Blocked.Layer != 3) bad |= 1;
    Node p; p.Size = 0.5f;                                      // prev half 0.25
    p.Position = V2(-0.5f, 1.25f);                              // above, inside the top edge: step right
    GetNextNeighbour(&n, &p, start);
    if (!Near(g_Asked, 0.0f, 1.25f)) bad |= 2;
    p.Position = V2(1.0f, 1.25f);                               // above, past the right edge: round the corner
    GetNextNeighbour(&n, &p, start);
    if (!Near(g_Asked, 1.25f, 0.75f)) bad |= 4;
    p.Position = V2(1.25f, 0.5f);                               // right side: step down
    GetNextNeighbour(&n, &p, start);
    if (!Near(g_Asked, 1.25f, 0.0f)) bad |= 8;
    p.Position = V2(1.25f, -1.0f);                              // right side, below the bottom: round the corner
    GetNextNeighbour(&n, &p, start);
    if (!Near(g_Asked, 0.75f, -1.25f)) bad |= 16;
    p.Position = V2(0.5f, -1.25f);                              // below: step left
    GetNextNeighbour(&n, &p, start);
    if (!Near(g_Asked, 0.0f, -1.25f)) bad |= 32;
    p.Position = V2(-1.0f, -1.25f);                             // below, past the left edge: round the corner
    GetNextNeighbour(&n, &p, start);
    if (!Near(g_Asked, -1.25f, -0.75f)) bad |= 64;
    p.Position = V2(-1.25f, -0.5f);                             // left side: step up
    GetNextNeighbour(&n, &p, start);
    if (!Near(g_Asked, -1.25f, 0.0f)) bad |= 128;
    p.Position = V2(-1.25f, 1.0f);                              // left side, above the top: back to the start
    g_ReturnHit = true; g_Hit.Position = start;
    r = GetNextNeighbour(&n, &p, start);
    if (!Near(g_Asked, -0.75f, 1.25f) || r != 0) bad |= 256;
    g_Hit.Position = V2(5.0f, 5.0f);                            // a different node there: returned
    r = GetNextNeighbour(&n, &p, start);
    if (r != &g_Hit) bad |= 512;
    if (bad) { std::printf("00a472f0_TEST FAIL %d\n", bad); return 1; }
    std::printf("00a472f0_TEST PASS\n");
    return 0;
}
