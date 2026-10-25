// NUISystem::CAction::AddParam(CActionParam* param, CCountedPointer<CAction>& self) -- append a UI parameter to the action; a component parameter is first told it is a parameter of this action (CComponent::AddActionIAmParamOf)
// Leaf: ?AddParam@CAction@NUISystem@@QAEXPAVCActionParam@2@AAV?$CCountedPointer@VCAction@NUISystem@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053FDE0, 71 bytes (+ int3 padding), thiscall `ret 8`.
// Identity: Ego_r.pdb `?AddParam@CAction@NUISystem@@QAEXPAVCActionParam@2@AAV?$CCountedPointer@VCAction@NUISystem@@@@@Z`
//   (and a one-argument overload); 287 call sites, the landed callers (CTextSlider::Initialize 0x00549B20,
//   CScrollableList::Initialise 0x00555EB0, CTCInventoryClothing / CTCInventoryTrade lists) declare it at this address
//   (some as a descriptive "SetTarget"). The catalog label "std::list::push_back" names the inlined callee.
// Layout: NUISystem::CAction (Ego_r.pdb: ActionType +0, UIActionParams list<CActionParam*> +4, ThingActionParams +8 in
//   retail -- STLport lists are one word --, Valid); NUISystem::CActionParam = vptr +0, ActionParamType +4; a
//   CComponent carries its CActionParam base at +0x18 (the landed callers pass `component ? component + 0x18 : 0`).
// Callees: CComponent::AddActionIAmParamOf(CCountedPointer<CAction>&), virtual slot 96 (+0x180; landed 0x00531E50),
//   malloc 0x00BFEA0E (the STLport node allocator; 12-byte node).
// Shape: `if (param->ActionParamType == 1) static_cast<CComponent&>(*param).AddActionIAmParamOf(self);
//   UIActionParams.push_back(param);` -- no null test on the downcast (`lea ecx,[edi-18h]`; the pointer form
//   `static_cast<CComponent*>(param)->...` grades the same: param was already dereferenced); STLport 4.6
//   push_back = insert(end(), x) inline: the head node is read before the allocation, the data is placement-new'd under
//   its null test, then the node is linked before the head through `prev = head->_M_prev`.
extern "C" void* __cdecl malloc(unsigned int size);          // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node<_Tp>* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator<_Tp, _Traits>& __x) : _List_iterator_base(__x._M_node) { }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;   // +0x00 (the sentinel)

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator end() { return iterator(this->_M_node._M_data); }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}

template <class T> class CCountedPointer {
public:
    T*    Data;                                              // +0x00
    void* Info;                                              // +0x04
};

namespace NUISystem {
class CAction;

class CActionParam {                                         // Ego_r.pdb, 8 bytes
public:
    virtual ~CActionParam();
    long ActionParamType;                                    // +0x04 (EActionParamType; 1 = a component)
};

class CComponentBase {                                       // the CComponent part before its CActionParam base
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void AddActionIAmParamOf(CCountedPointer<CAction>& action);   // slot 96 (+0x180), 0x00531E50
    unsigned char _pad04[0x18 - 0x04];
};
class CComponent : public CComponentBase, public CActionParam {   // CActionParam base at +0x18
};

class CAction {
public:
    long                          ActionType;                // +0x00
    std::list<CActionParam*>     UIActionParams;            // +0x04
    std::list<long>              ThingActionParams;         // +0x08
    bool                          Valid;                     // +0x0C
    void AddParam(CActionParam* param, CCountedPointer<CAction>& self);
};

void CAction::AddParam(CActionParam* param, CCountedPointer<CAction>& self)
{
    if (param->ActionParamType == 1)
        static_cast<CComponent&>(*param).AddActionIAmParamOf(self);
    UIActionParams.push_back(param);
}
}