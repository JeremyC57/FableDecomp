// Behaviour test for count_if<HasBuildingAsHomeOrWork> (0x006600EC): counts the elements the predicate accepts.
#include <cstdio>
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_node : _List_node_base { T _M_data; };
template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    T& operator*() const { return static_cast<_List_node<T>*>(_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& x) const { return _M_node != x._M_node; }
};
template <class _InputIter, class _Predicate>
long count_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{ long __n = 0; for ( ; __first != __last; ++__first) if (__pred(*__first)) ++__n; return __n; }
class CThing { public: int id; };
template <class T> class CIntelligentPointer { public: T* PItem; T* GetPItem() const { return PItem; } };
class CThingAICreature { public: CThing* Home; CThing* Work; };
class HasBuildingAsHomeOrWork { public: CThing* Building;
    bool operator()(const CIntelligentPointer<CThingAICreature>& c) const { return c.GetPItem() && (c.GetPItem()->Home == Building || c.GetPItem()->Work == Building); } };
int main()
{
    CThing house, other;
    CThingAICreature a, b, c; a.Home = &house; a.Work = &other; b.Home = &other; b.Work = &other; c.Home = &other; c.Work = &house;
    _List_node<CIntelligentPointer<CThingAICreature> > n[4]; _List_node_base head;
    CThingAICreature* items[4] = { &a, &b, &c, 0 };
    head._M_next = &n[0];
    for (int i = 0; i < 4; ++i) { n[i]._M_data.PItem = items[i]; n[i]._M_next = (i < 3) ? (_List_node_base*)&n[i + 1] : &head; }
    _List_iterator<CIntelligentPointer<CThingAICreature> > first, last; first._M_node = head._M_next; last._M_node = &head;
    HasBuildingAsHomeOrWork pred; pred.Building = &house;
    if (count_if(first, last, pred) != 2 || count_if(last, last, pred) != 0) { std::printf("006600ec_TEST FAIL\n"); return 1; }
    std::printf("006600ec_TEST PASS\n");
    return 0;
}