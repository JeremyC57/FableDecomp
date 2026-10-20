// Behaviour test for 0x00A76A80 CNavNavigableLeafNode::~CNavNavigableLeafNode: the node removes itself from each
// neighbour's list (swapped with the last entry, list shrunk by one), other entries keep their order otherwise.
#include <cstdio>

struct CNode;
struct CList {
    CNode* Data[8]; int Count;
    void resize(int n) { Count = n; }
};
struct CNode {
    CList Neighbours;
    CList* GetNeighbours() { return &Neighbours; }
    void Unlink()
    {
        for (CNode** it = Neighbours.Data; it != Neighbours.Data + Neighbours.Count; ++it) {
            CNode* neighbour = *it;
            CNode** self = neighbour->GetNeighbours()->Data;
            CNode** end = self + neighbour->GetNeighbours()->Count;
            while (self != end && *self != this) ++self;
            if (self - neighbour->GetNeighbours()->Data < neighbour->GetNeighbours()->Count - 1) {
                CNode* t = *self; *self = neighbour->GetNeighbours()->Data[neighbour->GetNeighbours()->Count - 1];
                neighbour->GetNeighbours()->Data[neighbour->GetNeighbours()->Count - 1] = t;
            }
            CList* list = neighbour->GetNeighbours();
            list->resize(list->Count - 1);
        }
    }
};
int main()
{
    CNode a, b, c, d;
    a.Neighbours.Count = 2; a.Neighbours.Data[0] = &b; a.Neighbours.Data[1] = &c;
    b.Neighbours.Count = 3; b.Neighbours.Data[0] = &a; b.Neighbours.Data[1] = &c; b.Neighbours.Data[2] = &d;
    c.Neighbours.Count = 2; c.Neighbours.Data[0] = &d; c.Neighbours.Data[1] = &a;
    a.Unlink();
    if (b.Neighbours.Count != 2 || b.Neighbours.Data[0] != &d || b.Neighbours.Data[1] != &c) { std::printf("00a76a80_TEST FAIL\n"); return 1; }
    if (c.Neighbours.Count != 1 || c.Neighbours.Data[0] != &d) { std::printf("00a76a80_TEST FAIL\n"); return 1; }
    std::printf("00a76a80_TEST PASS\n");
    return 0;
}