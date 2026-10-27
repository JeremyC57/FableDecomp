// Behaviour test for 0x008FC840 std::pair<float, std::list<C3DVector> >::pair(const pair&): copies the key and builds a
// fresh sentinel list holding copies of every point, in order.
#include <cstdio>
#include <cstdlib>

class C3DVector { public: float X, Y, Z; };
struct Node { Node* next; Node* prev; C3DVector v; };

struct PointList {
    Node* head;
    void Init() { head = (Node*)std::malloc(sizeof(Node)); head->next = head->prev = head; }
    void PushBack(const C3DVector& v)
    {
        Node* n = (Node*)std::malloc(sizeof(Node)); n->v = v;
        n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n;
    }
    void CopyFrom(const PointList& o)
    {
        Init();
        for (Node* p = o.head->next; p != o.head; p = p->next) PushBack(p->v);
    }
    void Free() { Node* p = head->next; while (p != head) { Node* n = p->next; std::free(p); p = n; } std::free(head); }
};
struct CurvePair {
    float first; PointList second;
    void CopyConstruct(const CurvePair& o) { first = o.first; second.CopyFrom(o.second); }
};

int main()
{
    CurvePair a; a.first = 0.75f; a.second.Init();
    C3DVector p1 = { 1, 2, 3 }, p2 = { 4, 5, 6 };
    a.second.PushBack(p1); a.second.PushBack(p2);
    CurvePair b; b.CopyConstruct(a);
    if (b.first != 0.75f || b.second.head == a.second.head) { std::printf("008fc840_TEST FAIL\n"); return 1; }
    Node* n = b.second.head->next;
    if (n == b.second.head || n->v.X != 1 || n->next->v.Z != 6 || n->next->next != b.second.head) { std::printf("008fc840_TEST FAIL\n"); return 1; }
    a.second.Free(); b.second.Free();
    CurvePair e; e.first = 1.0f; e.second.Init();
    CurvePair f; f.CopyConstruct(e);
    if (f.second.head->next != f.second.head) { std::printf("008fc840_TEST FAIL\n"); return 1; }
    e.second.Free(); f.second.Free();
    std::printf("008fc840_TEST PASS\n");
    return 0;
}