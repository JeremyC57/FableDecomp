// behaviour test for 0x0076D810 (STLport find_if over a list of intelligent thing pointers with a one-pointer predicate)
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
typedef CIntelligentPointer<const CThing> CThingPointer;
struct CIsHeldThing {
    const CBaseObject* m_pThing;
    bool operator()(const CThingPointer& p) const { return p.GetPItem() == m_pThing; }
};
namespace std {
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
template <class InputIter, class Predicate>
InputIter __fastcall find_if(InputIter first, InputIter last, Predicate pred)
{
    while (first != last && !pred(*first))
        ++first;
    return first;
}
typedef _List_iterator<CThingPointer, _Nonconst_traits<CThingPointer> > _ThingListIter;
}
typedef std::_ThingListIter It;
int main()
{
    CThing a, b, c;
    std::_List_node_base head; std::_List_node<CThingPointer> n1, n2, n3;
    head._M_next = &n1; n1._M_next = &n2; n2._M_next = &n3; n3._M_next = &head;
    n1._M_data.m_pItem = &a; n2._M_data.m_pItem = 0; n3._M_data.m_pItem = &b;
    CIsHeldThing pa, pb, pc, pnull; pa.m_pThing = &a; pb.m_pThing = &b; pc.m_pThing = &c; pnull.m_pThing = 0;
    int ok = std::find_if(It(&n1), It(&head), pb)._M_node == &n3
          && std::find_if(It(&n1), It(&head), pa)._M_node == &n1
          && std::find_if(It(&n1), It(&head), pc)._M_node == &head
          && std::find_if(It(&n1), It(&head), pnull)._M_node == &n2
          && std::find_if(It(&head), It(&head), pa)._M_node == &head;
    if (!ok) { printf("0076d810_TEST FAIL\n"); return 1; }
    printf("0076d810_TEST PASS\n");
    return 0;
}