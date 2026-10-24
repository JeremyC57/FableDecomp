// Behaviour test for 0x0058123A std::__copy_aux<list iterator, back_insert_iterator<vector>> (model copy)
#include <cstdio>
struct Rec { unsigned int id; };
struct Node { Node* next; Node* prev; Rec value; };
struct ListIter {
    Node* node;
    ListIter(Node* n) : node(n) {}
    ListIter(const ListIter& o) : node(o.node) {}
};
struct FakeVec { Rec items[8]; int count; void push_back(const Rec& x) { items[count++] = x; } };
struct BackInserter { FakeVec* container; explicit BackInserter(FakeVec& v) : container(&v) {} };
struct InputTag { InputTag() {} };
struct FalseType { FalseType() {} };
static BackInserter copy_input(ListIter first, ListIter last, BackInserter result, const InputTag&, long*)
{
    for (; first.node != last.node; first.node = first.node->next) result.container->push_back(first.node->value);
    return result;
}
static BackInserter copy_aux(ListIter first, ListIter last, BackInserter result, const FalseType&)
{
    return copy_input(first, last, result, InputTag(), (long*)0);
}
int main()
{
    Node head, a, b, c;
    head.next = &a; a.next = &b; b.next = &c; c.next = &head;
    a.value.id = 1; b.value.id = 2; c.value.id = 3;
    FakeVec v; v.count = 0;
    BackInserter r = copy_aux(ListIter(head.next), ListIter(&head), BackInserter(v), FalseType());
    if (r.container != &v || v.count != 3 || v.items[0].id != 1 || v.items[2].id != 3) { std::printf("0058123a_TEST FAIL\n"); return 1; }
    copy_aux(ListIter(&head), ListIter(&head), BackInserter(v), FalseType());
    if (v.count != 3) { std::printf("0058123a_TEST FAIL\n"); return 1; }
    std::printf("0058123a_TEST PASS\n");
    return 0;
}