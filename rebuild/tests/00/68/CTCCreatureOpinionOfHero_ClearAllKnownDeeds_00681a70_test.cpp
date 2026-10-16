#include <cstdio>
#include <cstdlib>
// Behaviour model of CTCCreatureOpinionOfHero::ClearAllKnownDeeds (0x00681A70): STLport-style list clear on 93 lists.
struct Node { Node* next; Node* prev; long v; };
struct List {
    Node* head;
    void init() { head = (Node*)std::malloc(sizeof(Node)); head->next = head->prev = head; }
    void push(long v) { Node* n = (Node*)std::malloc(sizeof(Node)); n->v = v; n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n; }
    void clear() { Node* c = head->next; while (c != head) { Node* t = c; c = c->next; std::free(t); } head->next = head; head->prev = head; }
    bool empty() const { return head->next == head && head->prev == head; }
};
int main()
{
    List pending; List known[92];
    pending.init(); pending.push(1); pending.push(2);
    for (int i = 0; i < 92; ++i) { known[i].init(); if (i % 3 == 0) known[i].push(i); }
    pending.clear();
    for (long i = 0; i < 92; ++i) known[i].clear();
    if (!pending.empty()) { std::printf("00681a70_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 92; ++i) if (!known[i].empty()) { std::printf("00681a70_TEST FAIL\n"); return 1; }
    std::printf("00681a70_TEST PASS\n");
    return 0;
}