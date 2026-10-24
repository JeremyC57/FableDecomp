// Behaviour test for 0x00A37940 list<CClass*>::operator=: overwrite the common prefix, then erase the surplus or append the rest.
#include <cstdio>
#include <cstdlib>

struct CClass { int id; };
struct Node { Node* next; Node* prev; CClass* data; };
struct List {
    Node* head;
    void Init() { head = (Node*)std::malloc(sizeof(Node)); head->next = head; head->prev = head; }
    Node* Insert(Node* n, CClass* x)
    {
        Node* tmp = (Node*)std::malloc(sizeof(Node));
        tmp->data = x;
        Node* p = n->prev;
        tmp->next = n; tmp->prev = p; p->next = tmp; n->prev = tmp;
        return tmp;
    }
    Node* Erase(Node* first, Node* last)
    {
        while (first != last) {
            Node* victim = first;
            first = first->next;
            victim->prev->next = victim->next;
            victim->next->prev = victim->prev;
            std::free(victim);
        }
        return last;
    }
    void InsertRange(Node* pos, Node* first, Node* last) { for (; first != last; first = first->next) Insert(pos, first->data); }
    List& Assign(const List& x)
    {
        if (this != &x) {
            Node* first1 = head->next;
            Node* last1 = head;
            Node* first2 = x.head->next;
            Node* last2 = x.head;
            while (first1 != last1 && first2 != last2) {
                first1->data = first2->data;
                first1 = first1->next;
                first2 = first2->next;
            }
            if (first2 == last2)
                Erase(first1, last1);
            else
                InsertRange(last1, first2, last2);
        }
        return *this;
    }
    int Size() const { int n = 0; for (Node* p = head->next; p != head; p = p->next) ++n; return n; }
    bool Same(const List& x) const
    {
        Node* a = head->next; Node* b = x.head->next;
        for (; a != head && b != x.head; a = a->next, b = b->next) if (a->data != b->data) return false;
        return a == head && b == x.head;
    }
};

int main()
{
    CClass c[6];
    List a, b, e;
    a.Init(); b.Init(); e.Init();
    for (int i = 0; i < 4; ++i) a.Insert(a.head, &c[i]);
    b.Insert(b.head, &c[5]);
    b.Insert(b.head, &c[4]);
    // shrink: a (4) = b (2)
    if (&a.Assign(b) != &a || a.Size() != 2 || !a.Same(b)) { std::printf("00a37940_TEST FAIL\n"); return 1; }
    // grow: b (2) = longer list
    List l; l.Init();
    for (int i = 0; i < 5; ++i) l.Insert(l.head, &c[i]);
    b.Assign(l);
    if (b.Size() != 5 || !b.Same(l)) { std::printf("00a37940_TEST FAIL\n"); return 1; }
    // self and empty
    b.Assign(b);
    if (b.Size() != 5) { std::printf("00a37940_TEST FAIL\n"); return 1; }
    b.Assign(e);
    if (b.Size() != 0 || b.head->next != b.head || b.head->prev != b.head) { std::printf("00a37940_TEST FAIL\n"); return 1; }
    std::printf("00a37940_TEST PASS\n");
    return 0;
}