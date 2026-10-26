// Behaviour test for 0x00946AE0 CAIStateGroup_LightHomeCandle::GetCandleInBuildingNeedingChanging: a copy of the
// source's logic over a model whose out-of-line callees (LowerBound, find_if, GetPItem, the predicate) have behaviour.
#include <cstdio>

class CTCBase { public: int Tag; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        int len = int(last - first);
        while (len > 0) {
            int half = len / 2;
            SEntry* middle = it + half;
            if (middle->type < *type) { it = middle + 1; len = len - half - 1; } else len = half;
        }
        return it;
    }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool NeedsChanging;                         // what the candle predicate asks (test model)
    CThing() : NeedsChanging(false) { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_interfaces.first = m_interfaces.last = 0; }
    template <class T> bool GetInterface(T** out, long type)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingBuilding : public CThing { };

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: };

namespace std {
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
};
template <class _Tp> struct _List_const_iterator : public _List_iterator_base {
    _List_const_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    const _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    const _Tp* operator->() const { return &(operator*()); }
    _List_const_iterator& operator++() { _M_incr(); return *this; }
};
inline bool operator==(const _List_iterator_base& __x, const _List_iterator_base& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _List_iterator_base& __x, const _List_iterator_base& __y) { return __x._M_node != __y._M_node; }
template <class _Tp> class list {
public:
    typedef _List_const_iterator<_Tp> const_iterator;
    _List_node_base* _M_node;
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
};
template <class _InputIter, class _Predicate>
_InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    while (__first != __last && !__pred(*__first))
        ++__first;
    return __first;
}
}

class CTCThingOwner : public CTCBase { public:
    std::list<CIntelligentPointer<CThing> > ObjectList;
};

class CAIStateGroup_LightHomeCandle;
class CIsCandleNeedingChanging { public:
    CAIStateGroup_LightHomeCandle* HomeCandle;
    CIsCandleNeedingChanging(const CAIStateGroup_LightHomeCandle* homeCandle)
        : HomeCandle(const_cast<CAIStateGroup_LightHomeCandle*>(homeCandle)) { }
    bool operator()(const CIntelligentPointer<CThing>& candle) const { return candle.GetPItem()->NeedsChanging; }
};

class CAIStateGroup_LightHomeCandle {
public:
    CThing* GetCandleInBuildingNeedingChanging(CThingBuilding& building) const;
};

CThing* CAIStateGroup_LightHomeCandle::GetCandleInBuildingNeedingChanging(CThingBuilding& building) const
{
    const CTCThingOwner* owner;
    if (building.GetInterface(&owner, 0x21)) {
        std::list<CIntelligentPointer<CThing> >::const_iterator it =
            std::find_if(owner->ObjectList.begin(), owner->ObjectList.end(), CIsCandleNeedingChanging(this));
        if (it != owner->ObjectList.end())
            return it->GetPItem();
    }
    return 0;
}

typedef std::_List_node<CIntelligentPointer<CThing> > TNode;
static void Link(std::_List_node_base* header, TNode* nodes, int count)
{
    std::_List_node_base* prev = header;
    for (int i = 0; i < count; ++i) { prev->_M_next = &nodes[i]; nodes[i]._M_prev = prev; prev = &nodes[i]; }
    prev->_M_next = header; header->_M_prev = prev;
}

int main()
{
    CAIStateGroup_LightHomeCandle group;
    CThing candles[3];
    TNode nodes[3];
    for (int i = 0; i < 3; ++i) nodes[i]._M_data.m_pItem = &candles[i];
    std::_List_node_base header;
    CTCThingOwner owner;
    owner.ObjectList._M_node = &header;
    CTCBase other1, other2;
    CInterfaceMap::SEntry entries[3] = { { 0x05, &other1 }, { 0x21, &owner }, { 0x40, &other2 } };

    CThingBuilding building;
    if (group.GetCandleInBuildingNeedingChanging(building) != 0) { std::printf("00946ae0_TEST FAIL\n"); return 1; }

    building.m_presentBits[1] = 1UL << 1;      // type 0x21
    building.m_interfaces.first = entries;
    building.m_interfaces.last = entries + 3;
    Link(&header, nodes, 0);
    if (group.GetCandleInBuildingNeedingChanging(building) != 0) { std::printf("00946ae0_TEST FAIL\n"); return 1; }
    Link(&header, nodes, 3);
    if (group.GetCandleInBuildingNeedingChanging(building) != 0) { std::printf("00946ae0_TEST FAIL\n"); return 1; }
    candles[1].NeedsChanging = true;
    candles[2].NeedsChanging = true;
    if (group.GetCandleInBuildingNeedingChanging(building) != &candles[1]) { std::printf("00946ae0_TEST FAIL\n"); return 1; }
    candles[0].NeedsChanging = true;
    if (group.GetCandleInBuildingNeedingChanging(building) != &candles[0]) { std::printf("00946ae0_TEST FAIL\n"); return 1; }
    std::printf("00946ae0_TEST PASS\n");
    return 0;
}