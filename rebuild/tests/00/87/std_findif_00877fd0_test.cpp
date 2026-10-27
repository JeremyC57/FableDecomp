// Behaviour test for 0x00877FD0 std::__find_if over list<const CThingBuilding*> with NCameraModes::CIsPosInBuilding: the
// first building whose IsPointInside(*Pos) holds, or last.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };
class Building {
public:
    float minX, maxX;
    bool IsPointInside(const C3DVector& p) const { return p.X >= minX && p.X <= maxX; }
};
struct Node { Node* next; Node* prev; const Building* data; };
class IsPosInBuilding {
public:
    C3DVector* Pos;
    bool operator()(const Building* b) const { return b->IsPointInside(*Pos); }
};
static Node* FindIf(Node* first, Node* last, IsPosInBuilding pred)
{
    while (first != last && !pred(first->data))
        first = first->next;
    return first;
}

int main()
{
    Building b0 = { 0.0f, 1.0f }, b1 = { 2.0f, 3.0f }, b2 = { 2.5f, 9.0f };
    Node head, n0, n1, n2;
    head.next = &n0; n0.next = &n1; n1.next = &n2; n2.next = &head;
    n0.data = &b0; n1.data = &b1; n2.data = &b2;
    C3DVector p = { 2.7f, 0.0f, 0.0f };
    IsPosInBuilding pred; pred.Pos = &p;
    if (FindIf(head.next, &head, pred) != &n1) { std::printf("00877fd0_TEST FAIL\n"); return 1; }
    p.X = 5.0f;
    if (FindIf(head.next, &head, pred) != &n2) { std::printf("00877fd0_TEST FAIL\n"); return 1; }
    p.X = -1.0f;
    if (FindIf(head.next, &head, pred) != &head) { std::printf("00877fd0_TEST FAIL\n"); return 1; }
    std::printf("00877fd0_TEST PASS\n");
    return 0;
}