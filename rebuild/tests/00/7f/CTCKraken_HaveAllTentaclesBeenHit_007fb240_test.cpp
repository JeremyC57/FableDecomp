#include <cstdio>
// Behaviour test: CTCKraken::HaveAllTentaclesBeenHit -- false before the fight starts (Phase 0); afterwards true once the Tentacles list is empty.
#define CHECK(c) if (!(c)) { std::printf("007fb240_TEST FAIL\n"); return 1; }
// Test model of the STLport list: a circular doubly-linked list with a head node; size() walks it.
struct SNode { SNode* next; SNode* prev; int data; };
struct CList {
    SNode head;
    CList() { head.next = &head; head.prev = &head; }
    unsigned int size() const { unsigned int n = 0; for (const SNode* p = head.next; p != &head; p = p->next) ++n; return n; }
    void push_back(SNode* n) { n->next = &head; n->prev = head.prev; head.prev->next = n; head.prev = n; }
    void erase(SNode* n) { n->prev->next = n->next; n->next->prev = n->prev; }
};

struct CTCKraken {
    CList Tentacles; long Phase;
    bool HaveAllTentaclesBeenHit() const { if (!Phase) return false; return Tentacles.size() == 0; }
};
int main()
{
    CTCKraken k; k.Phase = 0;
    CHECK(!k.HaveAllTentaclesBeenHit())
    k.Phase = 2; CHECK(k.HaveAllTentaclesBeenHit())
    SNode a, b; k.Tentacles.push_back(&a); k.Tentacles.push_back(&b);
    CHECK(!k.HaveAllTentaclesBeenHit())
    k.Tentacles.erase(&a); k.Tentacles.erase(&b);
    CHECK(k.HaveAllTentaclesBeenHit())
    k.Phase = 0; CHECK(!k.HaveAllTentaclesBeenHit())
    std::printf("007fb240_TEST PASS\n");
    return 0;
}