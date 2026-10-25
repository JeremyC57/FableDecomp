// CGameCameraManager::RemoveCameraPoint(CThing& thing) -- drop the thing from the available camera points; removing the first point requests a new automatic camera mode
// Leaf: ?RemoveCameraPoint@CGameCameraManager@@QAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006984A0, length 145 bytes (+ int3 padding), thiscall `ret 4`.
// Caller: CTCCameraPointShapeActivationBase::vfunc_9 0x0075C5F0 (landed; 0x0075C635, the subject left the shape).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90 and
//   ~CBaseIntelligentPointer @ 0x00A01C10 (the CIntelligentPointer<CThing> temporary, vtable 0x01238C6C),
//   _STL::__find(first, last, const CIntelligentPointer&, const input_iterator_tag&) @ 0x0069CAF0 (landed; __fastcall,
//   hidden iterator result), the node value's virtual destructor (slot 0, `push 0 ; call [eax]`), free @ 0x00BFEA14.
// Name: Ego_r.pdb `?RemoveCameraPoint@CGameCameraManager@@QAEXAAVCThing@@@Z`; catalog "std::list_erase_value" is
//   descriptive (label correction).
// Layout (retail, 4 below Ego_r.pdb, as the landed sibling AddCameraPoint 0x006991F0): NewAutoMode bool +0xA8,
//   AvailableCamPoints list<CIntelligentPointer<CThing> > +0xAC (STLport: the header node pointer only); the generated
//   engine/CGameCameraManager.h carries the Ego_r offsets (+0xAC / +0xB0, 12-byte list), so a local model is used.
// Shape: `it = find(AvailableCamPoints.begin(), AvailableCamPoints.end(), CIntelligentPointer<CThing>(&thing)); if (it
//   != AvailableCamPoints.end()) { if (it == AvailableCamPoints.begin()) NewAutoMode = true;
//   AvailableCamPoints.erase(it); }` -- STLport's inline find forwards to __find with the iterator-category tag (the
//   landed __find body as a same-TU noinline function homes the result and the tag in the dead `thing` slot); erase is
//   inline (unlink, `_Destroy(&node->_M_data)` = the virtual destructor call, node deallocate = free).
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CBaseObject;
class CThing;

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                     // slot 0; 0x00A01C10
    CBaseObject* m_pItem;                                   // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for T = CThing
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};
template <class T>
inline bool operator==(const CIntelligentPointer<T>& a, const CIntelligentPointer<T>& b) { return a.GetPItem() == b.GetPItem(); }

namespace _STL {
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };

template <class _Tp> class _Nonconst_traits { };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

class _List_node_base {
public:
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> class _List_node : public _List_node_base {
public:
    _Tp _M_data;
};
class _List_iterator_base {
public:
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> class _List_iterator : public _List_iterator_base {
public:
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
};
template <class _Tp, class _Traits>
inline bidirectional_iterator_tag iterator_category(const _List_iterator<_Tp, _Traits>&)
{
    return bidirectional_iterator_tag();
}

// __find @ 0x0069CAF0 (landed body): defined in the TU, never inlined, the tag parameter unnamed -- VC7.1 then homes the
// result iterator and the tag in the dead `thing` argument slot as retail (as the landed AddCameraPoint).
#pragma auto_inline(off)
template <class _InputIter, class _Tp>
__declspec(noinline) _InputIter __fastcall __find(_InputIter __first, _InputIter __last, const _Tp& __val,
                                                  const input_iterator_tag&)
{
    while (__first != __last && !(*__first == __val))
        ++__first;
    return __first;
}
#pragma auto_inline(on)
template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, iterator_category(__first));
}

template <class _Tp> class allocator { };

template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_node<_Tp>* _M_node;                               // header node
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        free(__n);                                          // _M_node.deallocate(__n, 1)
        return iterator((_Node*)__next_node);
    }
};
}

class CGameCameraManager {
public:
    unsigned char _pad00[0xA8];
    bool NewAutoMode;                                       // +0xA8
    unsigned char _padA9[3];
    _STL::list<CIntelligentPointer<CThing> > AvailableCamPoints;   // +0xAC
    void RemoveCameraPoint(CThing& thing);
};

void CGameCameraManager::RemoveCameraPoint(CThing& thing)
{
    _STL::list<CIntelligentPointer<CThing> >::iterator it =
        _STL::find(AvailableCamPoints.begin(), AvailableCamPoints.end(), CIntelligentPointer<CThing>(&thing));
    if (it != AvailableCamPoints.end()) {
        if (it == AvailableCamPoints.begin())
            NewAutoMode = true;
        AvailableCamPoints.erase(it);
    }
}