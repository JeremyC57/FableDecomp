// behaviour test for 0x0065DDB0 std::list<CIntelligentPointer<CThingBuilding> >::erase(iterator position): the node is unlinked (its neighbours point at each other), its element is destroyed exactly once through the virtual destructor, the node is freed, and the returned iterator is the following node (the sentinel after the last element)
#include <cstdio>
#include <cstdlib>
#include <new>
static int gDestroyed = 0;
static int gLastId = -2;
class CElem {                                    // stands for CIntelligentPointer<CThingBuilding>: a polymorphic element destroyed in place
public:
    int Id;
    CElem(int id) : Id(id) { }
    virtual ~CElem() { ++gDestroyed; gLastId = Id; }
};
struct Node { Node* next; Node* prev; CElem data; Node(int id) : data(id) { } };
static Node* NewNode(int id) { return new (std::malloc(sizeof(Node))) Node(id); }
static Node* Erase(Node* position)                // list::erase(iterator): returns the iterator to the next node
{
    Node* next = position->next;
    Node* prev = position->prev;
    prev->next = next;
    next->prev = prev;
    position->data.~CElem();
    std::free(position);
    return next;
}
static void PushBack(Node* s, Node* n) { n->next = s; n->prev = s->prev; s->prev->next = n; s->prev = n; }
static int Check(Node* s, const int* ids, int n)  // forward and backward walks see exactly ids[0..n)
{
    int k = 0;
    for (Node* p = s->next; p != s; p = p->next, ++k) {
        if (k >= n || p->data.Id != ids[k] || p->next->prev != p) return 0;
    }
    if (k != n) return 0;
    for (Node* q = s->prev; q != s; q = q->prev) {
        --k;
        if (k < 0 || q->data.Id != ids[k]) return 0;
    }
    return k == 0;
}
int main()
{
    int fails = 0;
    Node* s = NewNode(-1);
    s->next = s; s->prev = s;
    Node* n[4];
    for (int i = 0; i < 4; ++i) { n[i] = NewNode(i); PushBack(s, n[i]); }
    Node* r = Erase(n[1]);
    int a[3] = { 0, 2, 3 };
    if (r != n[2] || gDestroyed != 1 || gLastId != 1 || !Check(s, a, 3)) fails |= 1;
    r = Erase(n[3]);
    int b[2] = { 0, 2 };
    if (r != s || gDestroyed != 2 || gLastId != 3 || !Check(s, b, 2)) fails |= 2;
    r = Erase(n[0]);
    int c[1] = { 2 };
    if (r != n[2] || s->next != n[2] || gLastId != 0 || !Check(s, c, 1)) fails |= 4;
    r = Erase(n[2]);
    if (r != s || s->next != s || s->prev != s || gDestroyed != 4 || !Check(s, c, 0)) fails |= 8;
    if (s->data.Id != -1) fails |= 16;
    std::free(s);
    if (fails) { std::printf("0065ddb0_TEST FAIL %d\n", fails); return 1; }
    std::printf("0065ddb0_TEST PASS\n");
    return 0;
}