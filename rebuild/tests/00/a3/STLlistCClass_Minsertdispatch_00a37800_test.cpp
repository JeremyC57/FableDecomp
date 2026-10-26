// Behaviour test for 0x00A37800 list<CClass*>::_M_insert_dispatch: insert copies of [first, last) before pos, in order.
#include <cstdio>
#include <cstdlib>

class CClass { public: int id; };
struct Node { Node* next; Node* prev; CClass* data; };
struct List {
    Node* head;
    void Init() { head = (Node*)std::malloc(sizeof(Node)); head->next = head; head->prev = head; }
    Node* Insert(Node* n, CClass* x)
    {
        Node* tmp = (Node*)std::malloc(sizeof(Node));
        tmp->data = x;
        Node* p = n->prev;
        tmp->next = n;
        tmp->prev = p;
        p->next = tmp;
        n->prev = tmp;
        return tmp;
    }
    // the function under test
    void InsertDispatch(Node* pos, Node* first, Node* last)
    {
        for (; first != last; first = first->next)
            Insert(pos, first->data);
    }
};

int main()
{
    CClass c[5];
    for (int i = 0; i < 5; ++i) c[i].id = i;
    List a, b;
    a.Init(); b.Init();
    a.Insert(a.head, &c[0]);
    a.Insert(a.head, &c[4]);
    b.Insert(b.head, &c[1]);
    b.Insert(b.head, &c[2]);
    b.Insert(b.head, &c[3]);
    // insert all of b before a's second element (c[4])
    a.InsertDispatch(a.head->next->next, b.head->next, b.head);
    int expect[5] = { 0, 1, 2, 3, 4 };
    Node* n = a.head->next;
    for (int i = 0; i < 5; ++i, n = n->next) {
        if (n == a.head || n->data->id != expect[i] || n->next->prev != n) { std::printf("00a37800_TEST FAIL\n"); return 1; }
    }
    if (n != a.head || a.head->prev->data != &c[4]) { std::printf("00a37800_TEST FAIL\n"); return 1; }
    // empty range: nothing changes
    a.InsertDispatch(a.head, b.head, b.head);
    if (a.head->prev->data != &c[4]) { std::printf("00a37800_TEST FAIL\n"); return 1; }
    std::printf("00a37800_TEST PASS\n");
    return 0;
}