// behaviour test for 0x0069CAF0 (STLport __find over a list of intelligent thing pointers, value by reference)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class CBaseObject { public: int x; };
class CThing : public CBaseObject {};
struct CBaseIntelligentPointer {
    virtual void v00() {}
    CBaseObject* m_pItem;
    CBaseObject* GetPItem() const { return m_pItem; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
};
template <class T>
inline bool operator==(const CIntelligentPointer<T>& a, const CIntelligentPointer<T>& b) { return a.GetPItem() == b.GetPItem(); }
namespace std {
struct input_iterator_tag {};
template <class T> struct _Nonconst_traits {};
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_node : _List_node_base { T _M_data; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node == y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node != y._M_node; }
};
template <class T, class Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) {}
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) {}
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
};
template <class InputIter, class T>
InputIter __fastcall __find(InputIter first, InputIter last, const T& val, const input_iterator_tag&)
{
    while (first != last && !(*first == val))
        ++first;
    return first;
}
typedef CIntelligentPointer<const CThing> CThingPointer;
typedef _List_iterator<CThingPointer, _Nonconst_traits<CThingPointer> > _ThingListIter;
}
typedef std::_ThingListIter It;
typedef std::CThingPointer CThingPointer;
int main()
{
    CThing a, b, c;
    std::_List_node_base head; std::_List_node<CThingPointer> n1, n2, n3;
    head._M_next = &n1; n1._M_next = &n2; n2._M_next = &n3; n3._M_next = &head;
    n1._M_data.m_pItem = &a; n2._M_data.m_pItem = 0; n3._M_data.m_pItem = &b;   // n2 holds a dead thing
    CThingPointer pa, pb, pc, pdead; pa.m_pItem = &a; pb.m_pItem = &b; pc.m_pItem = &c; pdead.m_pItem = 0;
    std::input_iterator_tag tag;
    int ok = std::__find(It(&n1), It(&head), pb, tag)._M_node == &n3
          && std::__find(It(&n1), It(&head), pa, tag)._M_node == &n1
          && std::__find(It(&n1), It(&head), pc, tag)._M_node == &head
          && std::__find(It(&n1), It(&head), pdead, tag)._M_node == &n2
          && std::__find(It(&head), It(&head), pa, tag)._M_node == &head;
    if (!ok) { printf("0069caf0_TEST FAIL\n"); return 1; }
    printf("0069caf0_TEST PASS\n");
    return 0;
}