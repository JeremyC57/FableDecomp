#include <cstdio>
// Behaviour model of CVillageTaskPatrol::GetNearestNode: nearest node of the patrol chain, stopping before the end node.
struct V { float X, Y, Z; };
struct Node { V pos; Node* NextNode; bool IsEnd; };
static const Node* Nearest(const Node* start, const V& p)
{
    const Node* nearest = 0;
    const Node* node = start;
    if (node) {
        float best = 3.40282347e+38f;
        do {
            float dx = node->pos.X - p.X, dy = node->pos.Y - p.Y, dz = node->pos.Z - p.Z;
            float dist = dz * dz; dist += dx * dx; dist += dy * dy;
            if (dist < best) { best = dist; nearest = node; }
            node = node->NextNode;
        } while (node && !node->IsEnd);
    }
    return nearest;
}
int main()
{
    Node n[4];
    for (int i = 0; i < 4; ++i) { n[i].pos.X = 10.0f * i; n[i].pos.Y = 0; n[i].pos.Z = 0; n[i].NextNode = i < 3 ? &n[i + 1] : 0; n[i].IsEnd = false; }
    n[3].IsEnd = true;
    V p; p.X = 21.0f; p.Y = 0; p.Z = 0;
    if (Nearest(n, p) != &n[2]) { std::printf("00860a50_TEST FAIL\n"); return 1; }
    p.X = 100.0f;   // n[3] is the end node: never evaluated
    if (Nearest(n, p) != &n[2]) { std::printf("00860a50_TEST FAIL\n"); return 1; }
    if (Nearest(0, p) != 0) { std::printf("00860a50_TEST FAIL\n"); return 1; }
    std::printf("00860a50_TEST PASS\n");
    return 0;
}