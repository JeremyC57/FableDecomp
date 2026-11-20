#include <stdio.h>
// Behaviour test for 0x006604EB: STLport list::push_back(x) = insert(end(), x): the new node goes before the sentinel, after the last element.
struct Node { Node* next; Node* prev; int value; };
struct List {
    Node head;
    List() { head.next = &head; head.prev = &head; }
    Node* insert(Node* pos, const int& x) { Node* t = new Node; t->value = x; Node* p = pos->prev; t->next = pos; t->prev = p; p->next = t; pos->prev = t; return t; }
    void push_back(const int& x) { insert(&head, x); }
};
int main()
{
    List l; l.push_back(1); l.push_back(2); l.push_back(3);
    Node* n = l.head.next; int want = 1;
    for (; n != &l.head; n = n->next, ++want) if (n->value != want) return 1;
    if (want != 4 || l.head.prev->value != 3) return 1;
    puts("LISTPUSH_006604eb_PASS");
    return 0;
}
