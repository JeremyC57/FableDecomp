// Behaviour test for CBusyResourceManager::OnPostSwapScreens (0x00A618D0): the free list / pending list query recycling.
// Standalone copy of the logic over a minimal circular list (STLport list semantics: push_back, push_front, back, pop_back,
// size as a walk) and mock D3D query/device objects.
#include <cstdio>
#include <cstdlib>

struct Query {
    int  doneCalls;                       // GetData returns 0 (S_OK = finished) once doneCalls reaches 0
    int  issued;
    long GetData() { return doneCalls-- > 0 ? 1 : 0; }
    long Issue() { ++issued; return 0; }
};
struct Device {
    int  created;
    bool fail;
    Query pool[16];
    long CreateQuery(Query** out)
    {
        if (fail) return -1;
        pool[created].doneCalls = 0; pool[created].issued = 0;
        *out = &pool[created++];
        return 0;
    }
};
struct Pending { Query* Q; long Frame; };
struct Node { Node* next; Node* prev; Pending data; };
struct List {
    Node* head;
    List() { head = (Node*)std::malloc(sizeof(Node)); head->next = head->prev = head; }
    unsigned size() const { unsigned n = 0; for (Node* p = head->next; p != head; p = p->next) ++n; return n; }
    Pending& back() { return head->prev->data; }
    void insert(Node* pos, const Pending& v)
    {
        Node* n = (Node*)std::malloc(sizeof(Node)); n->data = v;
        n->next = pos; n->prev = pos->prev; pos->prev->next = n; pos->prev = n;
    }
    void push_back(const Pending& v) { insert(head, v); }
    void push_front(const Pending& v) { insert(head->next, v); }
    void pop_back() { Node* n = head->prev; n->prev->next = head; head->prev = n->prev; std::free(n); }
};
struct Manager {
    List Free, PendingQ;
    long PendingFrame, FinishedFrame;
    Device* Dev;
    void OnPostSwapScreens()
    {
        while (PendingQ.size() > 0 && (PendingQ.back().Q->GetData() == 0 || PendingQ.size() > 8)) {
            FinishedFrame = PendingQ.back().Frame;
            Free.push_back(PendingQ.back());
            PendingQ.pop_back();
        }
        if (Free.size() == 0) {
            Pending q;
            q.Frame = 0;                 // (uninitialised in the reconstruction; fixed here so the test is deterministic)
            if (Dev->CreateQuery(&q.Q) != 0) { FinishedFrame = PendingFrame - 2; return; }
            Free.push_back(q);
        }
        Pending q = Free.back();
        Free.pop_back();
        q.Q->Issue();
        PendingQ.push_front(q);
        ++PendingFrame;
    }
};

int main()
{
    Device dev; dev.created = 0; dev.fail = false;
    Manager m; m.PendingFrame = 1; m.FinishedFrame = 0; m.Dev = &dev;
    m.OnPostSwapScreens();                                   // empty free list: creates query 0, issues it
    bool ok = dev.created == 1 && m.PendingQ.size() == 1 && m.Free.size() == 0 && m.PendingFrame == 2 && dev.pool[0].issued == 1;
    dev.pool[0].doneCalls = 2;                               // query 0 not finished yet
    m.OnPostSwapScreens();                                   // still pending: a second query is created
    ok = ok && dev.created == 2 && m.PendingQ.size() == 2 && m.PendingFrame == 3;
    dev.pool[0].doneCalls = 0;                               // oldest (back) query finished: retired, recycled and reissued
    dev.pool[1].doneCalls = 5;
    m.PendingQ.back().Frame = 41;                            // the retired entry's frame becomes the finished frame
    m.OnPostSwapScreens();
    ok = ok && dev.created == 2 && m.FinishedFrame == 41 && m.PendingQ.size() == 2 && dev.pool[0].issued == 2;
    Device bad; bad.created = 0; bad.fail = true;
    Manager f; f.PendingFrame = 10; f.FinishedFrame = 0; f.Dev = &bad;
    f.OnPostSwapScreens();                                   // CreateQuery fails: finished frame = pending - 2, nothing issued
    ok = ok && f.FinishedFrame == 8 && f.PendingFrame == 10 && f.PendingQ.size() == 0;
    if (!ok) { std::printf("00a618d0_TEST FAIL\n"); return 1; }
    std::printf("00a618d0_TEST PASS\n");
    return 0;
}