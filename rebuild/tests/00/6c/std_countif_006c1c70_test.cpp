// Behaviour test for count_if<HasBuildingAsWork> (0x006C1C70): null, dead and other-workplace creatures are not counted.
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
class CThing { public: unsigned char m_isDead : 1; };
template <class T> class CIntelligentPointer { public: T* PItem; T* GetPItem() const { return PItem; } };
class CThingAICreature : public CThing { public: CIntelligentPointer<CThing> HomeBuilding; CIntelligentPointer<CThing> WorkBuilding;
    CThing* GetWorkBuilding() const { return WorkBuilding.GetPItem(); } };
class HasBuildingAsWork { public: CThing* Building;
    bool operator()(const CIntelligentPointer<CThingAICreature>& c) const { return c.GetPItem() && !c.GetPItem()->m_isDead && c.GetPItem()->GetWorkBuilding() == Building; } };
int main()
{
    CThing house, other; house.m_isDead = 0; other.m_isDead = 0;
    CThingAICreature a, b, c, d;
    a.m_isDead = 0; a.WorkBuilding.PItem = &house; b.m_isDead = 1; b.WorkBuilding.PItem = &house;
    c.m_isDead = 0; c.WorkBuilding.PItem = &other; d.m_isDead = 0; d.WorkBuilding.PItem = &house;
    _List_node<CIntelligentPointer<CThingAICreature> > n[6]; _List_node_base head;
    CThingAICreature* items[6] = { &a, &b, 0, &c, &d, &a };
    head._M_next = &n[0];
    for (int i = 0; i < 6; ++i) { n[i]._M_data.PItem = items[i]; n[i]._M_next = (i < 5) ? (_List_node_base*)&n[i + 1] : &head; }
    _List_iterator<CIntelligentPointer<CThingAICreature> > first, last; first._M_node = head._M_next; last._M_node = &head;
    HasBuildingAsWork pred; pred.Building = &house;
    long count = count_if(first, last, pred);
    long none = count_if(last, last, pred);
    if (count != 3 || none != 0) { std::printf("006c1c70_TEST FAIL\n"); return 1; }
    std::printf("006c1c70_TEST PASS\n");
    return 0;
}