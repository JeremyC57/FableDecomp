// Grade: DIFFER(170v170) -- frame layout only (sub esp,0Ch v 8): retail homes the find result iterator in the dead `thing` argument slot ([esp+24h] at the call, the same slot as the category tag) and keeps the CIntelligentPointer temporary at [esp+0Ch]; ours gives the named iterator its own frame word, so every temp offset is +4. Instruction sequence, calls and order are otherwise identical (the temporary is destroyed before the end() compare, as retail). Tried: one full-expression `find(...) == end()` (compare before the destructor, wrong order), named iterator (this), implicit iterator copy (argument copies change), direct-init iterator with `!(it != end())` (same as named).
// CGameCameraManager::AddCameraPoint(CThing& thing) -- add the thing to the available camera points unless it is already there; adding to an empty list requests a new automatic camera mode
// Leaf: ?AddCameraPoint@CGameCameraManager@@QAEXAAVCThing@@@Z
//
// Retail 0x006991F0, 170 bytes (+ int3 padding to 176), thiscall `ret 4`.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90 and
//   ~CBaseIntelligentPointer @ 0x00A01C10 (the CIntelligentPointer<CThing> temporaries, vtable 0x01238C6C),
//   std::__find(first, last, const CIntelligentPointer&, const input_iterator_tag&) @ 0x0069CAF0 (landed; __fastcall,
//   hidden iterator result), list<CIntelligentPointer<CThing> >::push_back(const CIntelligentPointer<CThing>&) @
//   0x004BA4E0 (out of line).
// Caller: 0x0075C65C.
// Layout (retail, 4 below Ego_r.pdb): NewAutoMode bool +0xA8, AvailableCamPoints list<CIntelligentPointer<CThing> >
//   +0xAC (STLport: the header node pointer only).
// Shape: `it = find(AvailableCamPoints.begin(), AvailableCamPoints.end(), CIntelligentPointer<CThing>(&thing)); if (it
//   == AvailableCamPoints.end()) { if (AvailableCamPoints.empty()) NewAutoMode = true;
//   AvailableCamPoints.push_back(CIntelligentPointer<CThing>(&thing)); }` -- STLport's inline find forwards to __find
//   with the iterator-category tag; the iterators are copy-constructed into their argument slots, the result and the
//   tag are homed in the dead `thing` argument slot, both temporaries share the 8-byte frame slot.
// Name: Ego_r.pdb `?AddCameraPoint@CGameCameraManager@@QAEXAAVCThing@@@Z`; catalog "FindInListAndInsertIfMatches" is
//   descriptive.
class CBaseObject;
class CThing;

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* m_pItem;                                   // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for T = CThing
public:
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
};

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };

template <class _Tp> class _Nonconst_traits { };

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
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> class _List_iterator : public _List_iterator_base {
public:
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
};
template <class _Tp, class _Traits>
inline bidirectional_iterator_tag iterator_category(const _List_iterator<_Tp, _Traits>&)
{
    return bidirectional_iterator_tag();
}

template <class _InputIter, class _Tp>
_InputIter __fastcall __find(_InputIter __first, _InputIter __last, const _Tp& __val,
                             const input_iterator_tag&);     // 0x0069CAF0
template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, iterator_category(__first));
}

template <class _Tp> class allocator { };

template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _List_node<_Tp>* _M_node;                               // header node
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    bool empty() const { return _M_node->_M_next == _M_node; }
    void push_back(const _Tp& __x);                         // 0x004BA4E0
};
}

class CGameCameraManager {
public:
    unsigned char _pad00[0xA8];
    bool NewAutoMode;                                       // +0xA8
    unsigned char _padA9[3];
    std::list<CIntelligentPointer<CThing> > AvailableCamPoints;   // +0xAC
    void AddCameraPoint(CThing& thing);
};

void CGameCameraManager::AddCameraPoint(CThing& thing)
{
    std::list<CIntelligentPointer<CThing> >::iterator it =
        std::find(AvailableCamPoints.begin(), AvailableCamPoints.end(), CIntelligentPointer<CThing>(&thing));
    if (it == AvailableCamPoints.end()) {
        if (AvailableCamPoints.empty())
            NewAutoMode = true;
        AvailableCamPoints.push_back(CIntelligentPointer<CThing>(&thing));
    }
}
