// std::_Push_heap(std::pair<long,CIntelligentPointer<CThing> >* first, int hole, int top, std::pair<long,CIntelligentPointer<CThing> > val, CKeyPairCompareLess<long,CIntelligentPointer<CThing> > pred) -- percolate a keyed intelligent thing pointer up a max-heap, then destroy the by-value pointer
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x006A2E20, 144 bytes incl. int3 padding (/O2). Called from the landed
// _Adjust_heap @ 0x006A3300 (same instantiation, same model). Callees: CBaseIntelligentPointer::
// GetPItem 0x00A01B50, SetPItem 0x00A01B90, ~CBaseIntelligentPointer 0x00A01C10.
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, rotated loop) over a 12-byte element:
// long key +0, CIntelligentPointer<CThing> +4 (vptr +4, item +8). The implicit pair operator= is the
// key move then CIntelligentPointer::operator= = `SetPItem(other.GetPItem())`; the key test is
// SIGNED (`jge`). The pointer's compiler-generated destructor stores no vtable and just calls the
// base destructor on `val.second` at the end. /Gr: first=ecx, hole=edx; `ret 14h` = top + 12-byte
// pair + the empty comparator.
namespace N006a2e20 {
class CThing;
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};
template<class K, class V> struct pair { K first; V second; };
template<class K, class V> struct CKeyPairCompareLess {
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Val); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    *(_First + _Hole) = _Val;
}
typedef pair<long, CIntelligentPointer<CThing> > Elem;
typedef CKeyPairCompareLess<long, CIntelligentPointer<CThing> > Pred;
template void _Push_heap<Elem*, int, Elem, Pred>(Elem*, int, int, Elem, Pred);
}