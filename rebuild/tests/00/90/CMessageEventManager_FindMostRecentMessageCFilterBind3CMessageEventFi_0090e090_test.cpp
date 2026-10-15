#include <cstdio>
struct Thing { unsigned int flags; int x; };
struct Event { int type; int origin; Thing* originator; };
struct Node { Node* next; Event value; };
struct IsOfType { int type; bool operator()(Event* e) const { return e->type == type; } };
struct IsOriginatedBy { int origin; bool operator()(Event* e) const { return e->origin == origin; } };
struct In2DRange { int maxX; bool operator()(Thing* t) const { return t->x <= maxX; } };
struct Search { const In2DRange* f;
    bool operator()(Event* e) const { if (e->originator && !(~e->originator->flags & 0x1f)) { Thing* t = e->originator; if (t) return (*f)(t); } return false; } };
struct Bind3 { const IsOfType* a; const IsOriginatedBy* b; const Search* c;
    bool operator()(Event* e) const { return (*a)(e) && (*b)(e) && (*c)(e); } };
static Event* FindMostRecent(Node* head, const Bind3& f) { for (Node* n = head->next; n != head; n = n->next) { Event* e = &n->value; if (f(e)) return e; } return 0; }
int main()
{
    Thing near_ = { 0x1f, 5 }, far_ = { 0x1f, 50 }, dead = { 0x0f, 1 };
    Node head, n1, n2, n3, n4;
    head.next = &n1; n1.next = &n2; n2.next = &n3; n3.next = &n4; n4.next = &head;
    n1.value.type = 2; n1.value.origin = 7; n1.value.originator = &near_;
    n2.value.type = 1; n2.value.origin = 7; n2.value.originator = &dead;
    n3.value.type = 1; n3.value.origin = 7; n3.value.originator = &far_;
    n4.value.type = 1; n4.value.origin = 7; n4.value.originator = &near_;
    IsOfType a = { 1 }; IsOriginatedBy b = { 7 }; In2DRange r = { 10 }; Search c = { &r }; Bind3 f = { &a, &b, &c };
    if (FindMostRecent(&head, f) != &n4.value) { std::printf("0090e090_TEST FAIL\n"); return 1; }
    r.maxX = 0; if (FindMostRecent(&head, f) != 0) { std::printf("0090e090_TEST FAIL\n"); return 1; }
    std::printf("0090e090_TEST PASS\n");
    return 0;
}