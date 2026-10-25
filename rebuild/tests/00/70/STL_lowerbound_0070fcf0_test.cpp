// Behaviour test for STLport __lower_bound over list<CAnimSet>::const_iterator by priority (0x0070FCF0).
#include <cstdio>

struct CAnimSet { long Priority; };
struct Node { Node* next; Node* prev; CAnimSet data; };
struct It { Node* node; const CAnimSet& operator*() const { return node->data; } };

struct CAnimSetItPriorityLessThan {
    bool operator()(const It& a, const It& b) const { return (*a).Priority < (*b).Priority; }
};

template <class F, class T, class C, class D>
F lower_bound_(F first, F last, const T& val, C comp, D*)
{
    D len = last - first;
    D half;
    F middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        middle += half;
        if (comp(*middle, val)) {
            first = middle;
            ++first;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return first;
}

int main()
{
    static const long prios[7] = { 1, 2, 2, 4, 7, 7, 9 };
    Node nodes[7];
    It its[7];
    for (int i = 0; i < 7; ++i) { nodes[i].data.Priority = prios[i]; its[i].node = &nodes[i]; }
    Node probe; It val; val.node = &probe;
    CAnimSetItPriorityLessThan comp;
    static const long keys[6] = { 0, 2, 3, 7, 9, 10 };
    static const int want[6] = { 0, 1, 3, 4, 6, 7 };
    for (int k = 0; k < 6; ++k) {
        probe.data.Priority = keys[k];
        It* r = lower_bound_(its, its + 7, val, comp, (int*)0);
        if (r - its != want[k]) { std::printf("0070fcf0_TEST FAIL\n"); return 1; }
    }
    if (lower_bound_(its, its, val, comp, (int*)0) != its) { std::printf("0070fcf0_TEST FAIL\n"); return 1; }
    std::printf("0070fcf0_TEST PASS\n");
    return 0;
}