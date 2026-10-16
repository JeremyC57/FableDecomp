#include <cstdio>
struct CBeam { long state; };
struct Node { Node* next; CBeam* beam; long* count; };
static bool AllInactive(Node* head)
{
    for (Node* it = head->next; it != head; it = it->next) {
        long* c = it->count; if (c) ++*c;
        bool bad = it->beam->state != 0 && it->beam->state != 6;
        if (c) --*c;
        if (bad) return false;
    }
    return true;
}
int main()
{
    long c1 = 1, c2 = 1; CBeam b0 = { 0 }, b6 = { 6 }, b3 = { 3 };
    Node head, n1, n2; head.next = &n1; n1.next = &n2; n2.next = &head;
    n1.beam = &b0; n1.count = &c1; n2.beam = &b6; n2.count = &c2;
    if (!AllInactive(&head)) { std::printf("0079f730_TEST FAIL\n"); return 1; }
    n2.beam = &b3;
    if (AllInactive(&head) || c1 != 1 || c2 != 1) { std::printf("0079f730_TEST FAIL\n"); return 1; }
    Node empty; empty.next = &empty;
    if (!AllInactive(&empty)) { std::printf("0079f730_TEST FAIL\n"); return 1; }
    std::printf("0079f730_TEST PASS\n");
    return 0;
}
