// behaviour test for 0x00BBDC60 (STLport list model)
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
    l.resize(3, h);
    if (l.size() != 3 || p.RefCount != 4) { std::printf("00bbdc60_TEST FAIL\n"); return 1; }
    l.resize(5, h);
    if (l.size() != 5 || p.RefCount != 6) { std::printf("00bbdc60_TEST FAIL\n"); return 1; }
    l.resize(2, h);
    if (l.size() != 2 || p.RefCount != 3) { std::printf("00bbdc60_TEST FAIL\n"); return 1; }
    l.resize(2, h);
    if (l.size() != 2 || p.RefCount != 3) { std::printf("00bbdc60_TEST FAIL\n"); return 1; }
    l.resize(0, h);
    if (l.size() != 0 || p.RefCount != 1) { std::printf("00bbdc60_TEST FAIL\n"); return 1; }
    std::printf("00bbdc60_TEST PASS\n");
    return 0;
}