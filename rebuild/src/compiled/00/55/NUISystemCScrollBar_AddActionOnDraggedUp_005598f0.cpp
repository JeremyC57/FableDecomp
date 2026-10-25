// NUISystem::CScrollBar::AddActionOnDraggedUp(CCountedPointer<NUISystem::CAction>& action) -- append a counted action to the component's on-dragged-up list, creating the (lazily allocated) STLport list on first use
// Leaf: ?AddActionOnDraggedUp@CScrollBar@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005598F0, 136 bytes (+ int3 padding), thiscall virtual, `ret 4`.
// Identity: no direct callers; it is slot 76 (+0x130) of the CScrollBar vtable 0x0124B7E4.
//   CComponent (vtable 0x0124608C) fills slots 66-80 with 15 empty `ret 4` stubs (0x0052EFF0..0x0052F0D0) defined in
//   the order 66-71, 80, 72-79 = the Ego_r CComponent stub order (AddAction, ..OnSelected, ..OnBack, AddPreAction, ..OnInactive,
//   ..OnLeftClicked, ..OnLeftUnclicked, ..OnLeftHeld, ..OnRightClicked, ..OnDropped, ..OnDroppedNowhere, ..OnDraggedUp,
//   ..OnDraggedDown, ..OnHovered, ..OnUnhovered); the retail overrides fit the Ego_r override sets (CDraggableInto and
//   CFrontEndButton replace only 66 = AddAction, CList 67/70, CSlider 66-68, CFrontEndScreen 71 = OnLeftClicked).
//   Ego_r name set: `?AddActionOnDraggedUp@CScrollBar@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z` (public virtual, a
//   non-const CCountedPointer<NUISystem::CAction>& argument -- the body reads Data and Info through the argument
//   pointer). Only the CScrollBar vtable holds this body.
//   Label correction: the manifest calls this NUISystem::CScrollBar::vfunc_76 (vtable-slot placeholder).
//   Family F010 (18 bodies identical but for the member offset).
// Callees: MSVCR71 operator new 0x00BFEA1A (the 4-byte list object), malloc 0x00BFEA0E (sentinel and node, 0x10 each).
// Layout: no PDB layout exists for the NUISystem component classes (struct_layouts_egor has none), so `this` is a
//   local model holding only the vptr and the list pointer at +0x1AC (the inherited CComponent part is padding); the member name is descriptive.
//   The list is a retail STLport 4.6 list (one sentinel pointer, 4 bytes) of 8-byte CCountedPointer {Data, Info}
//   whose copy constructor bumps Info->RefCount (CCPPointerInfo {RefCount, DeleteFunc, Data}, Ego_r).
// Shape: `if (!list) list = new std::list<...>; list->push_back(action);` with STLport 4.6 `_List_base` (proxy data
//   zeroed, then the sentinel allocated and self-linked) and push_back = insert(end(), x) (sentinel read before the node
//   is allocated, placement-new null test, four link stores). The dead `push ecx ; mov [esp+8],0` is the lifetime
//   slot of STLport's default allocator temporary: it appears only when `allocator` has its user destructor.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14 (unused here)
void* __cdecl operator new(unsigned int size);                                   // 0x00BFEA1A
inline void* operator new(unsigned int, void* p) { return p; }

class CCPPointerInfo {
public:
    unsigned long RefCount;                 // +0x00
    void (__cdecl* DeleteFunc)(void*);      // +0x04
    void* Data;                             // +0x08
};

template <class T> class CCountedPointer {
public:
    T* Data;                                // +0x00
    CCPPointerInfo* Info;                   // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

namespace std {

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    ~allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Tp1, class _Tp2> inline allocator<_Tp2>& __stl_alloc_rebind(allocator<_Tp1>& __a, const _Tp2*)
{
    return (allocator<_Tp2>&)(__a);
}
template <class _Tp1, class _Tp2> inline allocator<_Tp2> __stl_alloc_create(const allocator<_Tp1>&, const _Tp2*)
{
    return allocator<_Tp2>();
}

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
    typedef _MaybeReboundAlloc _Base;
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Tp* allocate(unsigned int __n) { return __stl_alloc_rebind((_Base&)*this, (_Tp*)0).allocate(__n, 0); }
    void deallocate(_Tp* __p, unsigned int __n) { __stl_alloc_rebind((_Base&)*this, (_Tp*)0).deallocate(__p, __n); }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp, class _Alloc> class _List_base {
protected:
    typedef _List_node<_Tp> _Node;
public:
    typedef _Alloc allocator_type;
    _List_base(const allocator_type& __a) : _M_node(__stl_alloc_create(__a, (_Node*)0), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
    typedef _List_node<_Tp> _Node;
public:
    typedef _Alloc allocator_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) { }
    void push_back(const _Tp& __x)
    {
        _List_node_base* __position = this->_M_node._M_data;
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __prev = __position->_M_prev;
        __tmp->_M_next = __position;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __position->_M_prev = __tmp;
    }
protected:
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
};

} // namespace std

namespace NUISystem {

class CAction;

class CScrollBar {                                    // retail: derives from NUISystem::CComponent (base layout not modelled)
public:
    virtual ~CScrollBar();
    unsigned char _pad004[0x1A8];
    std::list<CCountedPointer<CAction> >* ActionsOnDraggedUp;     // +0x1AC (descriptive name)
    virtual void AddActionOnDraggedUp(CCountedPointer<CAction>& action);   // vtable slot 76
};

void CScrollBar::AddActionOnDraggedUp(CCountedPointer<CAction>& action)
{
    if (!ActionsOnDraggedUp)
        ActionsOnDraggedUp = new std::list<CCountedPointer<CAction> >;
    ActionsOnDraggedUp->push_back(action);
}

} // namespace NUISystem