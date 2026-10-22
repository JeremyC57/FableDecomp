// Behaviour test for 004cd2d0 CPointerRecordList::push_back (insert at end of a circular list)
#include <cstdio>
struct SListNodeBase { SListNodeBase* _M_next; SListNodeBase* _M_prev; };
struct SPointerRecord { void* pointer; void* info; long value; };
struct SListNode : SListNodeBase { SPointerRecord _M_data; };
struct SListIterator {
    SListNodeBase* _M_node;
    SListIterator(SListNodeBase* node) : _M_node(node) {}
    SListIterator(const SListIterator& other) : _M_node(other._M_node) {}
};
struct CPointerRecordList {
    SListNodeBase* _M_node;
    SListIterator end() { return SListIterator(_M_node); }
    SListIterator insert(SListIterator position, const SPointerRecord& x)
    {
        SListNode* n = new SListNode; n->_M_data = x;
        n->_M_next = position._M_node; n->_M_prev = position._M_node->_M_prev;
        position._M_node->_M_prev->_M_next = n; position._M_node->_M_prev = n;
        return SListIterator(n);
    }
    void push_back(const SPointerRecord& x);
};
void CPointerRecordList::push_back(const SPointerRecord& x)
{
    insert(end(), x);
}
int main()
{
    SListNodeBase head; head._M_next = head._M_prev = &head;
    CPointerRecordList l; l._M_node = &head;
    for (long i = 1; i <= 3; ++i) { SPointerRecord r = { 0, 0, i }; l.push_back(r); }
    long want = 1;
    for (SListNodeBase* p = head._M_next; p != &head; p = p->_M_next, ++want)
        if (((SListNode*)p)->_M_data.value != want) { std::printf("004cd2d0_TEST FAIL\n"); return 1; }
    if (want != 4 || ((SListNode*)head._M_prev)->_M_data.value != 3) { std::printf("004cd2d0_TEST FAIL\n"); return 1; }
    while (head._M_next != &head) { SListNodeBase* p = head._M_next; head._M_next = p->_M_next; delete (SListNode*)p; }
    std::printf("004cd2d0_TEST PASS\n");
    return 0;
}
