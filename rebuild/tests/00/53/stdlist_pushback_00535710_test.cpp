// behaviour test for 0x00535710 std::list::push_back [list<CCountedPointer<NUISystem::CAction > >]: each push appends a node holding a copy of
// the counted pointer (the shared count goes up by one; a null info block is left alone) at the back, in order
#include <cstdio>
#include <cstdlib>
struct Info { long Count; };
struct CP { void* Data; Info* Info_; };
struct Node { Node* next; Node* prev; CP data; };
struct List {
    Node* node;
    List() { node = (Node*)std::malloc(sizeof(Node)); node->next = node; node->prev = node; }
    void PushBack(const CP& x)                          // the function under test
    {
        Node* n = node;
        Node* tmp = (Node*)std::malloc(sizeof(Node));
        tmp->data.Data = x.Data;
        tmp->data.Info_ = x.Info_;
        if (tmp->data.Info_) ++tmp->data.Info_->Count;
        Node* prev = n->prev;
        tmp->next = n;
        tmp->prev = prev;
        prev->next = tmp;
        n->prev = tmp;
    }
};
int main()
{
    int fail = 0;
    static int objs[3];
    Info shared = { 1 };
    List l;
    CP a = { &objs[0], &shared };
    CP b = { &objs[1], 0 };
    CP c = { &objs[2], &shared };
    l.PushBack(a); l.PushBack(b); l.PushBack(c);
    if (shared.Count != 3) fail |= 1;
    Node* p = l.node->next;
    if (p->data.Data != &objs[0] || p->next->data.Data != &objs[1] || p->next->next->data.Data != &objs[2]) fail |= 2;
    if (p->next->data.Info_ != 0) fail |= 4;
    if (l.node->prev->data.Data != &objs[2] || l.node->prev->next != l.node || p->prev != l.node) fail |= 8;
    if (fail) { std::printf("00535710_TEST FAIL %d\n", fail); return 1; }
    std::printf("00535710_TEST PASS\n");
    return 0;
}