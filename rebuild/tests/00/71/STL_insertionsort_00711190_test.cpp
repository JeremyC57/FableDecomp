// Behaviour test for 0x00711190 std::__insertion_sort over list<CAnimSet>::const_iterator with CAnimSetItPriorityLessThan (everything inlined).
#include <cstdio>

class CAnimSet { public: unsigned char _before[0x4c]; long Priority; unsigned char _after[0x0c]; };
struct SAnimSetNode { SAnimSetNode* _M_next; SAnimSetNode* _M_prev; CAnimSet _M_data; };
struct CAnimSetIterator {
    SAnimSetNode* _M_node;
    const CAnimSet& operator*() const { return _M_node->_M_data; }
};
class CAnimSetItPriorityLessThan { public:
    bool operator()(const CAnimSetIterator& a, const CAnimSetIterator& b) const { return (*a).Priority < (*b).Priority; }
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };

static CAnimSetIterator* copy_backward(CAnimSetIterator* first, CAnimSetIterator* last, CAnimSetIterator* result)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
static void __unguarded_linear_insert(CAnimSetIterator* last, CAnimSetIterator val, CAnimSetItPriorityLessThan comp)
{
    CAnimSetIterator* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
static void __linear_insert(CAnimSetIterator* first, CAnimSetIterator* last, CAnimSetIterator val, CAnimSetItPriorityLessThan comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}
static void __insertion_sort(CAnimSetIterator* first, CAnimSetIterator* last, CAnimSetItPriorityLessThan comp)
{
    if (first == last)
        return;
    for (CAnimSetIterator* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    static SAnimSetNode nodes[7];
    long prio[7] = { 5, 2, 9, 2, -1, 7, 5 };
    CAnimSetIterator it[7];
    for (int k = 0; k < 7; ++k) { nodes[k]._M_data.Priority = prio[k]; it[k]._M_node = &nodes[k]; }
    CAnimSetItPriorityLessThan comp;
    __insertion_sort(it, it + 7, comp);
    // ascending, stable: -1(4) 2(1) 2(3) 5(0) 5(6) 7(5) 9(2)
    int order[7] = { 4, 1, 3, 0, 6, 5, 2 };
    for (int j = 0; j < 7; ++j)
        if (it[j]._M_node != &nodes[order[j]]) { std::printf("00711190_TEST FAIL\n"); return 1; }
    for (int k = 0; k < 7; ++k)
        if (nodes[k]._M_data.Priority != prio[k]) { std::printf("00711190_TEST FAIL\n"); return 1; }
    std::printf("00711190_TEST PASS\n");
    return 0;
}