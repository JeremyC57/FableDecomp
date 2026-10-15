#include <cstdio>
struct Node;
struct Vec { const Node* d[8]; signed char n; void push_back(const Node* v) { d[n++] = v; } };
struct Node {
    long type; Vec Neighbours;
    long GetNodeType() const { return type; }
    Vec* GetNeighbours() { return &Neighbours; }
    void SetUpNeighboursWithLeaf(long, Node* leaf)
    {
        if (leaf->GetNodeType() == 1 || leaf->GetNodeType() == 3) {
            leaf->GetNeighbours()->push_back(this);
            Neighbours.push_back(leaf);
        }
    }
};
int main()
{
    Node a, b, c, d; a.type = 1; b.type = 3; c.type = 2; d.type = 1;
    a.Neighbours.n = b.Neighbours.n = c.Neighbours.n = d.Neighbours.n = 0;
    a.SetUpNeighboursWithLeaf(0, &b); a.SetUpNeighboursWithLeaf(0, &c); a.SetUpNeighboursWithLeaf(0, &d);
    if (a.Neighbours.n != 2 || a.Neighbours.d[0] != &b || a.Neighbours.d[1] != &d || b.Neighbours.n != 1 || b.Neighbours.d[0] != &a ||
        c.Neighbours.n != 0 || d.Neighbours.d[0] != &a) { std::printf("00a76b90_TEST FAIL\n"); return 1; }
    std::printf("00a76b90_TEST PASS\n");
    return 0;
}