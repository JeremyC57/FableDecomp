// NPlayerGui::CDrawPerceivers::AddPerceivingThing(CThing& thing) -- remember a thing that perceives the hero, unless it is already in the perceiver list
// Grade: DIFFER(83v83) -- one instruction scheduled differently: VC7.1 homes the `&thing` find-value temporary (`mov [ebp+8],ebx`) before the first iterator copy, retail after the second (right before the call); a pointer local and named iterator locals do not move it
// Leaf: ?AddPerceivingThing@CDrawPerceivers@NPlayerGui@@QAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0064A2DB, 83 bytes, thiscall, ebp frame, `leave ; ret 4`, /O1 TU.
// Identity (probable): the catalog label (Ego_r.pdb ?AddPerceivingThing@CDrawPerceivers@NPlayerGui@@QAEXAAVCThing@@@Z, donor 0x0068E572,
//   76 bytes, followed by RemovePerceivingThing); the body is exactly "find &thing in a list<CIntelligentPointer<const CThing>>,
//   push_back a new pointer when absent", the donor's _Find over list<CIntelligentPointer<const CThing>> with a `CThing* const&` value.
// Callees: std::find<list iterator, CThing*> 0x00655CDC (out of line in this TU, __fastcall: hidden result in ecx, `&value` in edx),
//   CIntelligentPointer<const CThing>::CIntelligentPointer(const CThing*) 0x0056C860, std::list<CIntelligentPointer<const CThing>>::
//   push_back(const value_type&) 0x0065617D, CBaseIntelligentPointer::~CBaseIntelligentPointer() 0x00A01C10 (virtual, called directly).
//   All four carry invented spellings in reloc_bindings (CThingListIterator find, CIntelligentPointerConstThing ctor, CRetailList
//   push_back, several dtor spellings) -> Review at landing.
// Layout: the perceiver list (STLport list, 4 bytes) at +0x3c; the member name is descriptive (CDrawPerceivers is padding in Ego_r).
#pragma optimize("s", on)
class CThing;

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();            // 0x00A01C10
    void* PData;                                   // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* p);                     // 0x0056C860 for <const CThing>
};

namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct _Nonconst_traits { };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    __forceinline _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    __forceinline _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    __forceinline _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    __forceinline bool operator==(const _List_iterator& __y) const { return _M_node == __y._M_node; }
};
template <class _Value, class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Value _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    __forceinline iterator begin() { return iterator(_M_node._M_data->_M_next); }
    __forceinline iterator end() { return iterator(_M_node._M_data); }
    void push_back(const _Tp& __x);                // 0x0065617D
};
template <class _InputIter, class _Tp> _InputIter __fastcall find(_InputIter __first, _InputIter __last, const _Tp& __val);   // 0x00655CDC
}

namespace NPlayerGui {

class CDrawPerceivers {
public:
    unsigned char _base[0x3c];                     // CDrawBase and the drawer's state
    std::list<CIntelligentPointer<const CThing> > PerceivingThings;   // +0x3c (descriptive)
    void AddPerceivingThing(CThing& thing);
};

void CDrawPerceivers::AddPerceivingThing(CThing& thing)
{
    if (std::find(PerceivingThings.begin(), PerceivingThings.end(), &thing) == PerceivingThings.end()) {
        CIntelligentPointer<const CThing> pointer(&thing);
        PerceivingThings.push_back(pointer);
    }
}

}
