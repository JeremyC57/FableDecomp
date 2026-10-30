// Behaviour test for 0x00AEB190 NParticleEngine::CParticleEmitterList::Insert: sorted insert by priority (high first) then
// distance (near first), with retail's "insert before the previous node" for hits after the first, and the maxCount cap
// on appends.
#include <cstdio>
#include <cstdlib>

struct Emitter { int Priority; float DistanceToCam; int id; };
struct Info { unsigned int RefCount; };
struct Ptr { Emitter* Data; Info* Info_; };
struct Node { Node* next; Node* prev; Ptr data; };
struct List { Node* head; };

static int Size(const List& l) { int n = 0; for (Node* p = l.head->next; p != l.head; p = p->next) ++n; return n; }
static void InsertBefore(Node* pos, const Ptr& x)
{
    Node* t = (Node*)std::malloc(sizeof(Node));
    t->data = x; if (x.Info_) ++x.Info_->RefCount;
    Node* p = pos->prev;
    t->next = pos; t->prev = p; p->next = t; pos->prev = t;
}
static bool Before(const Emitter* node, int priority, float distance)
{
    return node->Priority < priority || (node->Priority == priority && node->DistanceToCam > distance);
}
static bool Insert(List& visible, const Ptr& emitter, long maxCount)
{
    Node* it = visible.head->next;
    long count = Size(visible);
    float distance = emitter.Data->DistanceToCam;
    int priority = emitter.Data->Priority;
    if (count == 0 || Before(it->data.Data, priority, distance)) { InsertBefore(visible.head->next, emitter); return true; }
    for (it = it->next; it != visible.head; it = it->next)
        if (Before(it->data.Data, priority, distance)) { it = it->prev; InsertBefore(it, emitter); return true; }
    if (count < maxCount) { InsertBefore(visible.head, emitter); return true; }
    return false;
}

int main()
{
    int fail = 0;
    Node head; head.next = head.prev = &head;
    List l; l.head = &head;
    Info info = { 1 };
    Emitter e[6] = { { 2, 10.0f, 0 }, { 2, 5.0f, 1 }, { 1, 1.0f, 2 }, { 3, 50.0f, 3 }, { 0, 0.0f, 4 }, { 1, 0.5f, 5 } };
    Ptr p[6];
    for (int i = 0; i < 6; ++i) { p[i].Data = &e[i]; p[i].Info_ = &info; }
    if (!Insert(l, p[0], 3)) fail = 1;                 // empty -> front: [0]
    if (!Insert(l, p[1], 3)) fail = 1;                 // same priority, nearer -> front: [1 0]
    if (!Insert(l, p[2], 3)) fail = 1;                 // lower priority, no hit -> append: [1 0 2]
    if (!Insert(l, p[3], 3)) fail = 1;                 // higher priority -> front: [3 1 0 2]
    if (Insert(l, p[4], 3)) fail = 1;                  // lowest, count 4 >= 3 -> rejected
    if (!Insert(l, p[5], 9)) fail = 1;                 // hit at node 2 (index 3) -> before its previous node: [3 1 5 0 2]
    int want[5] = { 3, 1, 5, 0, 2 };
    int i = 0;
    for (Node* n = head.next; n != &head; n = n->next, ++i) if (i >= 5 || n->data.Data->id != want[i]) fail = 1;
    if (i != 5 || info.RefCount != 6) fail = 1;
    while (head.next != &head) { Node* n = head.next; head.next = n->next; std::free(n); }
    if (fail) { std::printf("00aeb190_TEST FAIL\n"); return 1; }
    std::printf("00aeb190_TEST PASS\n");
    return 0;
}