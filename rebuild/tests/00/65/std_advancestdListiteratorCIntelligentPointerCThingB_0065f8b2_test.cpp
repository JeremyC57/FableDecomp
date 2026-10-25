// Test for 0x0065F8B2 std::advance<_List_iterator<CIntelligentPointer<CThingBuilding> >,int>: the source object's
// advance must forward (i, n) to __advance, which steps the list iterator forward or back.
#include <stdio.h>
class CThing;
class CThingBuilding;
class CBaseIntelligentPointer { public: virtual void v00(); CThing* m_pItem; };
void CBaseIntelligentPointer::v00() {}
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

namespace std {
struct bidirectional_iterator_tag { bidirectional_iterator_tag() { } };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp> struct _List_iterator {
    typedef bidirectional_iterator_tag iterator_category;
    _List_node_base* _M_node;
};
static int g_calls;
template <class _BidirectionalIterator, class _Distance>
void __advance(_BidirectionalIterator& __i, _Distance __n, const bidirectional_iterator_tag&)
{
    ++g_calls;
    if (__n > 0) while (__n--) __i._M_node = __i._M_node->_M_next;
    else         while (__n++) __i._M_node = __i._M_node->_M_prev;
}
template void __advance(_List_iterator<CIntelligentPointer<CThingBuilding> >&, int, const bidirectional_iterator_tag&);
template <class _InputIterator, class _Distance> void advance(_InputIterator& __i, _Distance __n);
}

int main()
{
    std::_List_node_base n[4];
    for (int k = 0; k < 4; ++k) { n[k]._M_next = &n[(k + 1) & 3]; n[k]._M_prev = &n[(k + 3) & 3]; }
    std::_List_iterator<CIntelligentPointer<CThingBuilding> > it;
    it._M_node = &n[0];
    std::advance(it, 3);
    if (it._M_node != &n[3] || std::g_calls != 1) return 1;
    std::advance(it, -2);
    if (it._M_node != &n[1] || std::g_calls != 2) return 2;
    std::advance(it, 0);
    if (it._M_node != &n[1] || std::g_calls != 3) return 3;
    puts("0065f8b2_TEST PASS");
    return 0;
}
