// std::list::push_front(const T& x) -- (T = CCountedPointer<NParticleEngine::CParticleEmitter>) STLport list push_front: malloc a 16-byte node, placement copy-construct the counted emitter pointer into it (bumping the shared reference count) and link it in front of the first element
// Leaf: ?push_front@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEC740, 61 bytes, __fastcall `ret 4`.
// Callers: NParticleEngine::CParticleEmitterList::Insert 0x00AEB060 (catalog name; FableWin.pdb has Insert(CCountedPointer&)
//   and a private Insert(list&, const CCountedPointer&, long)): the call at 0x00AEB159 pushes onto the emitter list.
// Callees: malloc 0x00BFEA0E (the node allocator's allocate(1)).
// Layout: STLport list = one sentinel pointer; node = {_M_next, _M_prev, CCountedPointer<CParticleEmitter> (Data, count
//   block)} = 16 bytes; the count block's RefCount at +0.
// Label: catalog "List_InsertNode" is descriptive; the out-of-line insert(pos, x) of the same list is the landed 0x00AEC6F0.
// Shape: STLport 4.6 `push_front(x) { insert(begin(), x); }` with insert inline (`tmp = _M_create_node(x); n = pos._M_node;
//   p = n->_M_prev; tmp->_M_next = n; tmp->_M_prev = p; p->_M_next = tmp; n->_M_prev = tmp;`): begin() is read before the
//   node is allocated (it is the call's first argument), the placement new keeps its null test.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

namespace NParticleEngine { class CParticleEmitter; }

struct CCountBlock {
    long RefCount;                                          // +0x0
    void (__fastcall* DeleteData)(void* data);              // +0x4
    void* Data;                                             // +0x8
};

template <class T> class CCountedPointer {
public:
    T*           Data;                                      // +0x0
    CCountBlock* Block;                                     // +0x4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Block(other.Block)
    {
        if (Block)
            ++Block->RefCount;
    }
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

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
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
    void push_front(const _Tp& __x) { insert(begin(), __x); }
};
}

template void std::list<CCountedPointer<NParticleEngine::CParticleEmitter> >::push_front(const CCountedPointer<NParticleEngine::CParticleEmitter>&);