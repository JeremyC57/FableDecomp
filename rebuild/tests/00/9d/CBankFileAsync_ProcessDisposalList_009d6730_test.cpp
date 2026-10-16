// Behaviour test for 009d6730: CBankFileAsync::ProcessDisposalList -- pop from the front while the front's Abort() succeeds.
#include <cstdio>
#include <cstdlib>
struct TData { bool abortable; int aborts; bool Abort() { ++aborts; return abortable; } };
struct TNode { TNode* next; TNode* prev; TData* data; };
struct TList {
    TNode* node;
    TList() { node = (TNode*)std::malloc(sizeof(TNode)); node->next = node->prev = node; }
    ~TList() { while (size()) pop_front(); std::free(node); }
    unsigned long size() const { unsigned long n = 0; for (TNode* p = node->next; p != node; p = p->next) ++n; return n; }
    void push_back(TData* d) { TNode* n = (TNode*)std::malloc(sizeof(TNode)); n->data = d; n->next = node; n->prev = node->prev; node->prev->next = n; node->prev = n; }
    TData* front() { return node->next->data; }
    void pop_front() { TNode* n = node->next; n->prev->next = n->next; n->next->prev = n->prev; std::free(n); }
};
static int g_locks = 0;
struct TAsync { TList DisposalList;
    void ProcessDisposalList() { ++g_locks; while (DisposalList.size() != 0 && DisposalList.front()->Abort()) DisposalList.pop_front(); } };
int main()
{
    TData a = { true, 0 }, b = { true, 0 }, c = { false, 0 }, d = { true, 0 };
    TAsync x; x.DisposalList.push_back(&a); x.DisposalList.push_back(&b); x.DisposalList.push_back(&c); x.DisposalList.push_back(&d);
    x.ProcessDisposalList();
    if (x.DisposalList.size() != 2 || x.DisposalList.front() != &c || a.aborts != 1 || c.aborts != 1 || d.aborts != 0 || g_locks != 1) { std::printf("009d6730_TEST FAIL\n"); return 1; }
    c.abortable = true; x.ProcessDisposalList();
    if (x.DisposalList.size() != 0) { std::printf("009d6730_TEST FAIL\n"); return 1; }
    std::printf("009d6730_TEST PASS\n");
    return 0;
}