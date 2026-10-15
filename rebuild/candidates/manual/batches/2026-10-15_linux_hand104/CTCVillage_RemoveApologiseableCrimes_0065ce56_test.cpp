#include <cstdio>
// A sorted node chain stands in for the retail rb-tree: increment follows `next`, erase unlinks and poisons the node,
// so the test proves the walk advances before the call that may erase the current node.
struct SNode { SNode* next; SNode* prev; long value; bool erased; };
static SNode* RbIncrement(SNode* n) { return n->next; }
struct SCrimeIterator {
    SNode* _M_node;
    SCrimeIterator(SNode* n) : _M_node(n) { }
    long operator*() const { return _M_node->value; }
    SCrimeIterator& operator++() { _M_node = RbIncrement(_M_node); return *this; }
    bool operator!=(const SCrimeIterator& o) const { return _M_node != o._M_node; }
};
struct CTCVillage {
    SNode header; int count;
    void Erase(SNode* n) { n->prev->next = n->next; n->next->prev = n->prev; n->erased = true; n->next = 0; --count; }
    void RemoveCrimeIfApologisable(long crime)
    {
        for (SNode* n = header.next; n != &header; n = n->next)
            if (n->value == crime) { if (crime % 2 == 0) Erase(n); return; }
    }
    void RemoveApologiseableCrimes(long)
    {
        SCrimeIterator last(&header);
        SCrimeIterator it(header.next);
        SCrimeIterator next = it;
        while (it != last) {
            ++next;
            RemoveCrimeIfApologisable(*it);
            it = next;
        }
    }
};
int main()
{
    SNode nodes[8]; CTCVillage v; v.header.next = v.header.prev = &v.header; v.count = 0;
    for (int i = 0; i < 8; ++i) {
        nodes[i].value = i + 1; nodes[i].erased = false;
        nodes[i].prev = v.header.prev; nodes[i].next = &v.header; v.header.prev->next = &nodes[i]; v.header.prev = &nodes[i]; ++v.count;
    }
    v.RemoveApologiseableCrimes(0);
    if (v.count != 4 || !nodes[1].erased || !nodes[7].erased || nodes[6].erased) { std::printf("0065ce56_TEST FAIL\n"); return 1; }
    std::printf("0065ce56_TEST PASS\n");
    return 0;
}
