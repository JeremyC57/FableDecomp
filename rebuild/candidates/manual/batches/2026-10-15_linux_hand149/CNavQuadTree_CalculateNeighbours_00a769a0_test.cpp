#include <cstdio>
struct Node {
    int id; int children; int right; int up;
    void CalculateChildrenNeighbours() { ++children; }
    void SetUpNeighbours(long dir, Node* n) { if (dir == 2) right = n->id; else up = n->id; }
};
struct Vec { Node** b; Node** e; unsigned int size() { return (unsigned int)(e - b); } Node*& operator[](unsigned int n) { return *(b + n); } };
struct Tree {
    float MapWidth;
    void CalculateNeighbours(Vec& nodes) const
    {
        unsigned int width = (unsigned int)(MapWidth * 0.03125f);
        for (unsigned int i = 0; i < nodes.size(); ) {
            Node* node = nodes[i];
            node->CalculateChildrenNeighbours();
            ++i;
            if (i % width != 0) node->SetUpNeighbours(2, nodes[i]);
            if (i > width) node->SetUpNeighbours(1, nodes[i - width - 1]);
        }
    }
};
int main()
{
    Node n[6]; Node* p[6];
    for (int i = 0; i < 6; ++i) { n[i].id = i; n[i].children = 0; n[i].right = -1; n[i].up = -1; p[i] = &n[i]; }
    Vec v; v.b = p; v.e = p + 6;
    Tree t; t.MapWidth = 96.0f;    // 3 nodes wide, 2 rows
    t.CalculateNeighbours(v);
    bool fail = false;
    int right[6] = { 1, 2, -1, 4, 5, -1 }, up[6] = { -1, -1, -1, 0, 1, 2 };
    for (int i = 0; i < 6; ++i) if (n[i].children != 1 || n[i].right != right[i] || n[i].up != up[i]) fail = true;
    if (fail) { std::printf("00a769a0_TEST FAIL\n"); return 1; }
    std::printf("00a769a0_TEST PASS\n");
    return 0;
}
