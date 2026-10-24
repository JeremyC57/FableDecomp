// CAnimateThing::CDelayedAnimation::CDelayedAnimation() -- default constructor: an empty STLport list<CAnimSet> of combination anims (a fresh self-linked 0x5C-byte sentinel node)
// Leaf: ??0CDelayedAnimation@CAnimateThing@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00711D50, length 30 bytes (+ int3 padding). __fastcall, plain `ret`.
//         FableWin.pdb names `??0CDelayedAnimation@CAnimateThing@@QAE@XZ`; Ego_r.pdb: CAnimateThing::CDelayedAnimation
//         = { list<CAnimSet> CombinationAnims } (retail 4 bytes: the sentinel pointer).
// Callees: malloc (CRT import thunk) @ 0x00BFEA0E -- the 0x5C-byte sentinel (8 bytes of links + the 0x54-byte retail CAnimSet).
// Identity: the manifest label "std_list_ctor" is generic. A no-argument constructor (plain `ret`, no allocator argument as
//   the landed list<T> ctor 0x00711C30 takes) of a class whose only member is a list<CAnimSet>: CDelayedAnimation, in the
//   CAnimateThing TU block; ClearAllAnims 0x0070EC30 inlines the same body for its queue refill. No direct callers.
// Shape: STLport `_List_base(const allocator_type&)`: the proxy's `_M_data(0)` store first, then the sentinel built in a
//   local (self-linked through eax) and stored last -- writing `_M_node._M_data = allocate(1)` first makes VC7.1 store
//   then reload it (`mov [esi],eax ; mov eax,[esi]`).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)

template <class _Tp> class allocator {                  // STLport allocator over malloc / free (empty)
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) {}
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
class CAnimSet {
public:
    unsigned char _body[0x54];                          // retail CAnimSet (Ego_r.pdb 0x5C minus 8)
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp> class _List_base {
public:
    typedef allocator<_Tp> allocator_type;
    _STLP_alloc_proxy<_List_node<_Tp>*, allocator<_List_node<_Tp> > > _M_node;
    _List_base(const allocator_type&) : _M_node(allocator<_List_node<_Tp> >(), (_List_node<_Tp>*)0)
    {
        _List_node<_Tp>* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
};
template <class _Tp> class list : public _List_base<_Tp> {
public:
    typedef allocator<_Tp> allocator_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp>(__a) {}
};

class CAnimateThing {
public:
    class CDelayedAnimation {
    public:
        list<CAnimSet> CombinationAnims;                // +0x00
        CDelayedAnimation();
    };
};

CAnimateThing::CDelayedAnimation::CDelayedAnimation()
{
}