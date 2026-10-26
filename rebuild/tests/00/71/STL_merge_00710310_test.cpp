// Behaviour test for 0x00710310 std::merge of two sorted runs of list<CAnimSet>::const_iterator by the anim set's Priority.
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
static CAnimSetIterator* copy(CAnimSetIterator* first, CAnimSetIterator* last, CAnimSetIterator* result)
{
    for (int n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
static CAnimSetIterator* merge(CAnimSetIterator* first1, CAnimSetIterator* last1, CAnimSetIterator* first2, CAnimSetIterator* last2,
                               CAnimSetIterator* result, CAnimSetItPriorityLessThan comp)
{
    while (first1 != last1 && first2 != last2) {
        if (comp(*first2, *first1)) { *result = *first2; ++first2; }
        else { *result = *first1; ++first1; }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}

int main()
{
    static SAnimSetNode nodes[7];
    long prio[7] = { -3, 2, 8, /* run 2 */ 2, 3, 9, 10 };
    CAnimSetIterator it[7], out[7];
    for (int k = 0; k < 7; ++k) { nodes[k]._M_data.Priority = prio[k]; it[k]._M_node = &nodes[k]; }
    CAnimSetItPriorityLessThan comp;
    if (merge(it, it + 3, it + 3, it + 7, out, comp) != out + 7) { std::printf("00710310_TEST FAIL\n"); return 1; }
    // -3(0) 2(1) 2(3) 3(4) 8(2) 9(5) 10(6)
    int order[7] = { 0, 1, 3, 4, 2, 5, 6 };
    for (int j = 0; j < 7; ++j)
        if (out[j]._M_node != &nodes[order[j]]) { std::printf("00710310_TEST FAIL\n"); return 1; }
    std::printf("00710310_TEST PASS\n");
    return 0;
}