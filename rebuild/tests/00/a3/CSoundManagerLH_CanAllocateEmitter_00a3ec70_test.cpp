// Behaviour test for CSoundManagerLH::CanAllocateEmitter (0x00A3EC70): true while the live-emitter list holds fewer entries
// than the emitter limit (+0x18). Host model over a circular list with a sentinel, as STLport walks it for size().
#include <cstdio>
struct Node { Node* next; };
struct Manager {
    unsigned int MaxEmitters; Node* Sentinel;
    unsigned int Count() const { unsigned int n = 0; for (Node* p = Sentinel->next; p != Sentinel; p = p->next) ++n; return n; }
    bool CanAllocateEmitter() const { return Count() < MaxEmitters; }
};
int main()
{
    Node head, a, b; head.next = &head;
    Manager m; m.Sentinel = &head; m.MaxEmitters = 2;
    bool ok = m.CanAllocateEmitter();
    head.next = &a; a.next = &b; b.next = &head;
    ok = ok && !m.CanAllocateEmitter();
    m.MaxEmitters = 3; ok = ok && m.CanAllocateEmitter();
    if (!ok) { std::printf("00a3ec70_TEST FAIL\n"); return 1; }
    std::printf("00a3ec70_TEST PASS\n");
    return 0;
}
