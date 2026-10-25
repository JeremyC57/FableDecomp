// Behaviour test for 0x0070FD30 _STL::__upper_bound over list<CAnimSet>::const_iterator ordered by Priority.
#include <cstdio>

struct CAnimSet {
    unsigned char _pad00[0x4c];
    long          Priority;
};
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { CAnimSet data; };
struct It {
    NodeBase* node;
    const CAnimSet& operator*() const { return ((Node*)node)->data; }
};
struct PriorityLess {
    bool operator()(const It& a, const It& b) const { return (*a).Priority < (*b).Priority; }
};

template <class F, class T, class C, class D>
F upper_bound_model(F first, F last, const T& val, C comp, D*)
{
    D len = last - first;
    D half;
    F middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        middle += half;
        if (comp(val, *middle))
            len = half;
        else {
            first = middle;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}

int main()
{
    static Node nodes[6];
    long prios[6] = { 1, 3, 3, 5, 7, 9 };
    It its[6];
    for (int i = 0; i < 6; ++i) { nodes[i].data.Priority = prios[i]; its[i].node = &nodes[i]; }
    Node key; It k; k.node = &key;
    long probes[7] = { 0, 1, 3, 4, 7, 9, 10 };
    int expect[7] = { 0, 1, 3, 3, 5, 6, 6 };
    for (int i = 0; i < 7; ++i) {
        key.data.Priority = probes[i];
        It* r = upper_bound_model(its, its + 6, k, PriorityLess(), (int*)0);
        if (r - its != expect[i]) { std::printf("0070fd30_TEST FAIL\n"); return 1; }
    }
    if (upper_bound_model(its, its, k, PriorityLess(), (int*)0) != its) { std::printf("0070fd30_TEST FAIL\n"); return 1; }
    std::printf("0070fd30_TEST PASS\n");
    return 0;
}