#include <cstdio>
#include <cstdlib>
// Host model of NUISystem::CObservable::RemoveAllObservers: clear two circular lists and the count.
struct Node { Node* next; Node* prev; };
struct List {
    Node* node;
    List() { node = (Node*)std::malloc(sizeof(Node)); node->next = node->prev = node; }
    void Add() { Node* n = (Node*)std::malloc(sizeof(Node)); n->prev = node->prev; n->next = node; node->prev->next = n; node->prev = n; }
    void clear() { Node* cur = node->next; while (cur != node) { Node* tmp = cur; cur = cur->next; std::free(tmp); } node->next = node; node->prev = node; }
    bool empty() const { return node->next == node && node->prev == node; }
};
struct Observable {
    List observers; long count; List pending;
    void RemoveAllObservers() { observers.clear(); count = 0; pending.clear(); }
};
int main()
{
    Observable o; o.observers.Add(); o.observers.Add(); o.count = 2; o.pending.Add();
    o.RemoveAllObservers();
    if (!o.observers.empty() || !o.pending.empty() || o.count != 0) { std::printf("0055c950_TEST FAIL\n"); return 1; }
    std::printf("0055c950_TEST PASS\n");
    return 0;
}
