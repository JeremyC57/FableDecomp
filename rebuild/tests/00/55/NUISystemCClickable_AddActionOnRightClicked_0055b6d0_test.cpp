// behaviour test for 0x0055B6D0 NUISystem::CClickable::AddActionOnRightClicked: the list is created once on first use, actions are appended in order with the sentinel links intact, each stored copy adds one reference, a null Info is copied without a count
#include <cstdio>
#include <cstdlib>
#include <stddef.h>
inline void* operator new(size_t, void* p, int) { return p; }
static int gMallocs = 0;
static void* CountedMalloc(size_t n) { ++gMallocs; return std::malloc(n); }
class CCPPointerInfo {
public:
    unsigned int RefCount;
    void (*DeleteFunc)(void*);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* data, CCPPointerInfo* info) : Data(data), Info(info) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};
namespace stlp_test {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p, 0) _T1(__val); }
template <class _Tp> class allocator {
public:
    allocator() { }
    ~allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)CountedMalloc(__n * sizeof(_Tp)) : 0; }
};
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp> class list {
public:
    typedef _List_node<_Tp> _Node;
    allocator<_Node> _M_alloc;
    _List_node_base* _M_data;
    list()
    {
        _M_data = 0;
        _Node* __n = _M_alloc.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_data = __n;
    }
    ~list()
    {
        _List_node_base* __p = _M_data->_M_next;
        while (__p != _M_data) { _List_node_base* __n = __p->_M_next; std::free(__p); __p = __n; }
        std::free(_M_data);
    }
    void push_back(const _Tp& __x)
    {
        _List_node_base* __position = _M_data;
        _Node* __tmp = _M_alloc.allocate(1);
        _Construct(&__tmp->_M_data, __x);
        _List_node_base* __prev = __position->_M_prev;
        __tmp->_M_next = __position;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __position->_M_prev = __tmp;
    }
};
}
namespace NUISystem {
class CAction { public: int Id; };
class CClickable {
public:
    stlp_test::list<CCountedPointer<CAction> >* ActionsOnRightClicked;
    CClickable() : ActionsOnRightClicked(0) { }
    virtual ~CClickable() { delete ActionsOnRightClicked; }
    virtual void AddActionOnRightClicked(CCountedPointer<CAction>& action)
    {
        if (!ActionsOnRightClicked)
            ActionsOnRightClicked = new stlp_test::list<CCountedPointer<CAction> >;
        ActionsOnRightClicked->push_back(action);
    }
};
}
using namespace NUISystem;
typedef stlp_test::_List_node<CCountedPointer<CAction> > TNode;
static int Check()
{
    CAction a1, a2, a3;
    a1.Id = 1; a2.Id = 2; a3.Id = 3;
    CCPPointerInfo i1 = { 1, 0, &a1 }, i3 = { 5, 0, &a3 };
    CCountedPointer<CAction> p1(&a1, &i1), p2(&a2, 0), p3(&a3, &i3);
    CClickable* c = new CClickable;
    if (c->ActionsOnRightClicked != 0) return 1;
    c->AddActionOnRightClicked(p1);
    stlp_test::list<CCountedPointer<CAction> >* l = c->ActionsOnRightClicked;
    if (l == 0 || gMallocs != 2) return 2;
    if (i1.RefCount != 2) return 3;
    c->AddActionOnRightClicked(p2);
    c->AddActionOnRightClicked(p3);
    if (c->ActionsOnRightClicked != l || gMallocs != 4) return 4;
    if (i3.RefCount != 6 || i1.RefCount != 2) return 5;
    stlp_test::_List_node_base* s = l->_M_data;
    TNode* n1 = (TNode*)s->_M_next;
    TNode* n2 = (TNode*)n1->_M_next;
    TNode* n3 = (TNode*)n2->_M_next;
    if (n3->_M_next != s || s->_M_prev != n3 || n3->_M_prev != n2 || n2->_M_prev != n1 || n1->_M_prev != s) return 6;
    if (n1->_M_data.Data != &a1 || n1->_M_data.Info != &i1) return 7;
    if (n2->_M_data.Data != &a2 || n2->_M_data.Info != 0) return 8;
    if (n3->_M_data.Data->Id != 3 || n3->_M_data.Info != &i3) return 9;
    delete c;
    return 0;
}
int main()
{
    int r = Check();
    if (r != 0) { std::printf("0055b6d0_TEST FAIL %d\n", r); return 1; }
    std::printf("0055b6d0_TEST PASS\n");
    return 0;
}