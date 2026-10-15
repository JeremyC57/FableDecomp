#include <cstdio>
#include <cstdlib>
static int g_frees = 0;
static void my_free(void* p) { ++g_frees; std::free(p); }
struct NodeBase { NodeBase* next; NodeBase* prev; };
template <class T> struct Node : NodeBase { T data; };
template <class T> struct List {
    NodeBase* head;
    void clear()
    {
        Node<T>* cur = (Node<T>*)head->next;
        while (cur != head) { Node<T>* tmp = cur; cur = (Node<T>*)cur->next; tmp->data.~T(); my_free(tmp); }
        head->next = head; head->prev = head;
    }
    ~List() { clear(); if (head) my_free(head); }
};
struct Entry { long first; List<long> second; };
template <class T> static Node<T>* Add(NodeBase* head)
{
    Node<T>* n = (Node<T>*)std::malloc(sizeof(Node<T>));
    n->prev = head->prev; n->next = head; head->prev->next = n; head->prev = n; return n;
}
static NodeBase* NewHead() { NodeBase* h = (NodeBase*)std::malloc(sizeof(NodeBase)); h->next = h->prev = h; return h; }
int main()
{
    List<Entry> l; l.head = NewHead();
    for (int i = 0; i < 2; ++i) {
        Node<Entry>* e = Add<Entry>(l.head); e->data.first = i; e->data.second.head = NewHead();
        for (int j = 0; j <= i; ++j) Add<long>(e->data.second.head)->data = j;
    }
    l.clear();
    bool fail = l.head->next != l.head || l.head->prev != l.head || g_frees != 2 + 2 + 3;
    if (fail) { std::printf("00850280_TEST FAIL\n"); return 1; }
    std::printf("00850280_TEST PASS\n");
    return 0;
}
