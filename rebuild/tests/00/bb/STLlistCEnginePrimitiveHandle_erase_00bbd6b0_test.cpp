// behaviour test for 0x00BBD6B0 (STLport list model)
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
    Prim p = { 0 };
    List<Handle> l;
    Handle h(&p);
    l.fill_insert(l.head, 5, h);
    if (p.RefCount != 6 || l.size() != 5) { std::printf("00bbd6b0_TEST FAIL\n"); return 1; }
    Node<Handle>* first = l.head->next->next;       // erase elements 2..4 (keep the first)
    Node<Handle>* r = l.erase(first, l.head);
    if (r != l.head || l.size() != 1 || p.RefCount != 2 || g_destroyed != 4) { std::printf("00bbd6b0_TEST FAIL\n"); return 1; }
    if (l.erase(l.head, l.head) != l.head || l.size() != 1) { std::printf("00bbd6b0_TEST FAIL\n"); return 1; }
    std::printf("00bbd6b0_TEST PASS\n");
    return 0;
}