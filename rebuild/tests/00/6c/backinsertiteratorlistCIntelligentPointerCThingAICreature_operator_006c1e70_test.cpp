// Behaviour test for 0x006C1E70 back_insert_iterator<list<CIntelligentPointer<T>>>::operator=(const value_type&).
#include <cstdio>
#include <cstdlib>
#include <new>
struct Object { int id; };
struct Record { Object* item; long refs; };
static int g_records = 0;
struct IPtr {                                   // CIntelligentPointer: shares one {item, refs} record per object
    Record* data;
    IPtr() : data(0) { }
    IPtr(const IPtr& o) : data(0) { Set(o.Get()); }
    ~IPtr() { Clear(); }
    void Clear() { if (data) { if (--data->refs == 0) { delete data; --g_records; } data = 0; } }
    Object* Get() const { return data ? data->item : 0; }
    void Set(Object* p) { Clear(); if (p) { data = new Record; ++g_records; data->item = p; data->refs = 1; } }
};
struct Node { Node* next; Node* prev; IPtr value; };
struct List {
    Node* head;
    List() { head = (Node*)std::malloc(sizeof(Node)); head->next = head->prev = head; }
    void push_back(const IPtr& x) {
        Node* pos = head;                       // end() before the node is created
        Node* tmp = (Node*)std::malloc(sizeof(Node));
        if (&tmp->value) new (&tmp->value) IPtr(x);
        Node* prev = pos->prev;
        tmp->next = pos; tmp->prev = prev; prev->next = tmp; pos->prev = tmp;
    }
    ~List() {
        Node* n = head->next;
        while (n != head) { Node* nx = n->next; n->value.~IPtr(); std::free(n); n = nx; }
        std::free(head);
    }
};
struct BackInserter {
    List* container;
    BackInserter& operator=(const IPtr& v) { container->push_back(v); return *this; }
};
int main() {
    int ok = 1;
    {
        Object a = { 7 }, b = { 9 };
        IPtr pa; pa.Set(&a);
        IPtr pb; pb.Set(&b);
        IPtr empty;
        List l;
        BackInserter it = { &l };
        BackInserter* r = &(it = pa);
        ok &= r == &it;
        it = pb;
        it = empty;
        it = pa;
        int ids[4] = { 7, 9, -1, 7 }, n = 0;
        for (Node* p = l.head->next; p != l.head; p = p->next, ++n) {
            Object* o = p->value.Get();
            ok &= n < 4 && (o ? o->id : -1) == ids[n];
            ok &= p->next->prev == p && p->prev->next == p;
        }
        ok &= n == 4 && l.head->prev->value.Get() == &a && l.head->next->value.Get() == &a;
        ok &= g_records == 2 + 3;               // two originals + three non-empty copies (fresh records in the model)
    }
    ok &= g_records == 0;
    if (!ok) { std::printf("006c1e70_TEST FAIL\n"); return 1; }
    std::printf("006c1e70_TEST PASS\n");
    return 0;
}