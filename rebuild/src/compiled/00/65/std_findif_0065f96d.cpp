// std::__find_if(std::list<CCountedPointer<CVillageTaskHorn> >::iterator first, std::list<CCountedPointer<CVillageTaskHorn> >::iterator last, CIsHornTaskWithThisHorn pred, const std::input_iterator_tag&) -- finds the first horn task bound to a given horn thing (out-of-line by-value predicate)
// Leaf: __find_if
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065F96D, 62 bytes, `ret 10h`. Caller: the STLport find_if front end 0x0065FFCA
// (by-value predicate copied with 0x004A9730, destroyed with 0x00A01C10; called from CTCVillage::UpdateHornTasks). Callees: CCountedPointer
// copy ctor @ 0x0065D89B (argument built in its slot), CIsHornTaskWithThisHorn::operator()(CCountedPointer<CVillageTaskHorn>) @
// 0x0065F116 (manifest), ~CBaseIntelligentPointer @ 0x00A01C10 (the by-value predicate's CIntelligentPointer member, destroyed
// after the result store). Size TU (`#pragma optimize("s", on)`); VC picks the ebp frame itself here (many [ebp+N] parameter
// references are shorter than [esp+N]). Model: STLport 4.x _List_node/_List_iterator (the one-argument iterator model of
// the landed sibling __find_if 0x0065EA75 / IsThingInTaskList 0x0065FF5E); the earlier Dinkumware SListNode/CListIterator
// stand-ins were wrong (FINDINGS chunk 383) -- retail STL is STLport.
#pragma optimize("s", on)
class CThing;
class CVillageTaskHorn;
namespace std {
struct input_iterator_tag {};
}
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
private:

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
template <class T> class CCountedPointer {
public:
    T* m_pObject;
    void* m_pInfo;
    CCountedPointer(const CCountedPointer& other);   // 0x0065D89B (out of line)
    ~CCountedPointer();                           // 0x0065D5A0 (out of line, run by the callee)
};
namespace std {
struct _List_node_base {                          // STLport 4.x list node (retail STL is STLport, not Dinkumware)
    _List_node_base* _M_next;                     // +0x00
    _List_node_base* _M_prev;                     // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                  // +0x08
};
template <class _Tp> struct _List_iterator {      // list<T>::iterator: one node pointer with a user copy ctor
    _List_node_base* _M_node;
    _List_iterator(_List_node<_Tp>* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
}
namespace std {
template <class _InputIter, class _Predicate>
_InputIter __fastcall __find_if(_InputIter __first, _InputIter __last, _Predicate __pred, const input_iterator_tag&)
{
    while (__first != __last && !__pred(*__first))
        ++__first;
    return __first;
}
}
class CIsHornTaskWithThisHorn {
public:
    CIntelligentPointer<CThing> m_horn;           // +0x00
    bool operator()(CCountedPointer<CVillageTaskHorn> task) const;   // 0x0065F116 (out of line)
};
template std::_List_iterator<CCountedPointer<CVillageTaskHorn> > __fastcall std::__find_if(std::_List_iterator<CCountedPointer<CVillageTaskHorn> >,
    std::_List_iterator<CCountedPointer<CVillageTaskHorn> >, CIsHornTaskWithThisHorn, const std::input_iterator_tag&);