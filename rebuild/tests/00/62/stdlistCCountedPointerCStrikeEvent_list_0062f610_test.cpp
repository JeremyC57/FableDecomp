#include <cstdio>
#include <cstdlib>
// Test model: STLport list<CCountedPointer<CStrikeEvent> > with a malloc'd self-linked sentinel; the counted pointer copy adds a reference.
struct CCPPointerInfo { long RefCount; };
struct CStrikeEvent { int Frame; };
struct CCountedPointer {
    CStrikeEvent* Data; CCPPointerInfo* Info;
    CCountedPointer(CStrikeEvent* d, CCPPointerInfo* i) : Data(d), Info(i) { ++Info->RefCount; }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { ++Info->RefCount; }
};
inline void* operator new(std::size_t, void* where) { return where; }
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
struct _List_node : _List_node_base { CCountedPointer _M_data; };
struct TestList {
    _List_node_base* _M_node;
    _List_node_base* insert(_List_node_base* position, const CCountedPointer& x)
    {
        _List_node* tmp = (_List_node*)std::malloc(sizeof(_List_node));
        if (&tmp->_M_data) new (&tmp->_M_data) CCountedPointer(x);
        _List_node_base* n = position;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    TestList()
    {
        _M_node = (_List_node_base*)std::malloc(sizeof(_List_node));
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    int size() const { int n = 0; for (_List_node_base* p = _M_node->_M_next; p != _M_node; p = p->_M_next) ++n; return n; }
    const CCountedPointer& at(int i) const { _List_node_base* p = _M_node->_M_next; while (i--) p = p->_M_next; return ((_List_node*)p)->_M_data; }
};

// The copy constructor: a fresh sentinel (inlined _List_base ctor), then insert(begin(), x.begin(), x.end()) 0x0062F650.
struct TestListCopy : TestList {
    TestListCopy(const TestList& x)
    {
        for (_List_node_base* p = x._M_node->_M_next; p != x._M_node; p = p->_M_next)
            insert(_M_node, ((_List_node*)p)->_M_data);
    }
};
int main()
{
    CCPPointerInfo info = { 0 };
    CStrikeEvent ev[3] = { {10}, {20}, {30} };
    TestList src;
    for (int i = 0; i < 3; ++i) src.insert(src._M_node, CCountedPointer(&ev[i], &info));
    long before = info.RefCount;
    TestListCopy copy(src);
    if (copy.size() != 3 || copy._M_node == src._M_node) { std::printf("0062f610_TEST FAIL\n"); return 1; }
    for (int k = 0; k < 3; ++k) if (copy.at(k).Data != &ev[k]) { std::printf("0062f610_TEST FAIL\n"); return 1; }
    if (info.RefCount != before + 3) { std::printf("0062f610_TEST FAIL\n"); return 1; }
    TestList empty;
    TestListCopy copy2(empty);
    if (copy2.size() != 0 || copy2._M_node->_M_next != copy2._M_node) { std::printf("0062f610_TEST FAIL\n"); return 1; }
    std::printf("0062f610_TEST PASS\n");
    return 0;
}