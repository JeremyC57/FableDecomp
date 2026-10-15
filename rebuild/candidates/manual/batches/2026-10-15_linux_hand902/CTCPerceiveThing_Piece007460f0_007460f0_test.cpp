#include <cstdio>
// Behaviour test for 0x007460F0: STLport-style list back() == sentinel->_M_prev->_M_data.
struct PerceiveListNode {
    PerceiveListNode* _M_next;
    PerceiveListNode* _M_prev;
    long              _M_data;
};
struct PerceiveList {
    PerceiveListNode* _M_node;
    long& back();
};
long& PerceiveList::back() { return _M_node->_M_prev->_M_data; }

int main()
{
    PerceiveListNode sentinel, n1, n2;
    // circular list: sentinel <-> n1 <-> n2 <-> sentinel
    sentinel._M_next = &n1; sentinel._M_prev = &n2; sentinel._M_data = -1;
    n1._M_next = &n2; n1._M_prev = &sentinel; n1._M_data = 11;
    n2._M_next = &sentinel; n2._M_prev = &n1; n2._M_data = 22;
    PerceiveList l; l._M_node = &sentinel;
    if (l.back() != 22 || &l.back() != &n2._M_data) { std::printf("007460f0_TEST FAIL\n"); return 1; }
    l.back() = 33;
    if (n2._M_data != 33) { std::printf("007460f0_TEST FAIL\n"); return 1; }
    std::printf("007460f0_TEST PASS\n");
    return 0;
}
