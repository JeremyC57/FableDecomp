// NParticleEngine::CParticleEmitterList::CParticleEmitterList(long maxActiveEmitters) -- empty active-emitter list (STLport list of counted emitter pointers: a self-linked sentinel) with its emitter cap
// Leaf: ??0CParticleEmitterList@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEB940, 39 bytes, __fastcall `ret 4`, returns this. No direct caller (the manager
//   constructor 0x00AEBA90 inlines it with 0x80; this is the out-of-line copy).
// Identity: Ego_r.pdb `??0CParticleEmitterList@NParticleEngine@@QAE@J@Z`; members Active (list<CCountedPointer<CParticleEmitter> >)
//   and MaxActiveEmitters (PDB +0xc; retail +4: the STLport list is one pointer).
// Callees: malloc 0x00BFEA0E (STLport allocator<_Node>::allocate(1): the 0x10-byte sentinel = two links + an 8-byte pointer).
// Label: catalog "StdList_ConstructWithArg_aeb940" is descriptive.
// Shape: STLport 4.6 `_List_base(const allocator_type& a) : _M_node(a, (_Node*)0) { n = _M_node.allocate(1); n->_M_next =
//   n; n->_M_prev = n; _M_node._M_data = n; }` (the proxy zeroed first), then the cap from the initializer list.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

namespace NParticleEngine { class CParticleEmitter; }
struct CCountBlock;
template <class T> class CCountedPointer {
public:
    T*           Data;                                      // +0x0
    CCountBlock* Block;                                     // +0x4
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
    _List_base(const allocator_type& __a) : _M_node(allocator<_Node>(__a), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) { }
};
}

namespace NParticleEngine {
class CParticleEmitterList {
public:
    std::list<CCountedPointer<CParticleEmitter> > Active;  // +0x0
    long MaxActiveEmitters;                                 // +0x4
    CParticleEmitterList(long maxActiveEmitters);
};

CParticleEmitterList::CParticleEmitterList(long maxActiveEmitters)
    : MaxActiveEmitters(maxActiveEmitters)
{
}
}