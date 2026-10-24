// behaviour test for 0x00BBD770 (STLport list model)
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_destroyed = 0;
struct Prim { int RefCount; };
struct Handle {                                  // CEnginePrimitiveHandle model: counted pointer, add-ref on copy
    Prim* Data;
    Handle(Prim* p) : Data(p) { if (Data) ++Data->RefCount; }
    Handle(const Handle& o) : Data(o.Data) { if (Data) ++Data->RefCount; }
    virtual ~Handle() { if (Data) --Data->RefCount; ++g_destroyed; }
};

template <class T> struct Node { Node* next; Node* prev; T data; };
template <class T> struct List {                 // STLport list: a pointer to the sentinel node
    Node<T>* head;
    List() { head = (Node<T>*)std::malloc(sizeof(Node<T>)); head->next = head; head->prev = head; }
    unsigned size() const { unsigned n = 0; for (Node<T>* p = head->next; p != head; p = p->next) ++n; return n; }
    void fill_insert(Node<T>* pos, unsigned n, const T& x)
    {
        for (; n > 0; --n) {
            Node<T>* tmp = (Node<T>*)std::malloc(sizeof(Node<T>));
            new (&tmp->data) T(x);
            Node<T>* p = pos->prev;
            tmp->next = pos; tmp->prev = p; p->next = tmp; pos->prev = tmp;
        }
    }
    Node<T>* erase(Node<T>* first, Node<T>* last)
    {
        while (first != last) {
            Node<T>* n = first;
            first = first->next;
            n->prev->next = n->next;
            n->next->prev = n->prev;
            n->data.~T();
            std::free(n);
        }
        return last;
    }
    void resize(unsigned newSize, const T& x)
    {
        Node<T>* i = head->next;
        unsigned len = 0;
        for (; i != head && len < newSize; i = i->next, ++len)
            ;
        if (len == newSize)
            erase(i, head);
        else
            fill_insert(head, newSize - len, x);
    }
};

int main()
{
    List<long> l;
    long v = 3;
    l.fill_insert(l.head, 2, v);
    long w = 5;
    l.fill_insert(l.head->next->next, 3, w);       // between the two 3s
    long expect[5] = { 3, 5, 5, 5, 3 };
    int i = 0;
    for (Node<long>* p = l.head->next; p != l.head; p = p->next, ++i)
        if (i >= 5 || p->data != expect[i]) { std::printf("00bbd770_TEST FAIL\n"); return 1; }
    if (i != 5) { std::printf("00bbd770_TEST FAIL\n"); return 1; }
    for (Node<long>* q = l.head->prev; q != l.head; q = q->prev) --i;   // back links consistent
    if (i != 0) { std::printf("00bbd770_TEST FAIL\n"); return 1; }
    std::printf("00bbd770_TEST PASS\n");
    return 0;
}