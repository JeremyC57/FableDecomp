// CMessageEventManager::CMessageEventManager(const CWorld& world) -- store the owning world and construct the empty STLport message list (allocate its 0x60-byte sentinel and link it to itself)
// Leaf: ??0CMessageEventManager@@QAE@ABVCWorld@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687510, 40 bytes, thiscall, `ret 4`, returns this.
// Identity: the manifest's "std::list_node::Initialize" is propagated. Ego_r.pdb names
//   ??0CMessageEventManager@@QAE@ABVCWorld@@@Z; the only caller is CWorld::Init 0x004A6E30 (`new` of 8 bytes, the
//   result handed to the CCountedPointer<CMessageEventManager> at CWorld+0x60). Ego_r.pdb CMessageEventManager:
//   World +0 (CWorld*), Messages +4 (list<CMessageEvent>; CMessageEvent is 0x58 bytes, so the node is 0x60).
// Callees: malloc 0x00BFEA0E (STLport node allocator, allocate(1) of a _List_node<CMessageEvent>).
// Shape: STLport `_List_base(const allocator_type& a) : _M_node(a, (_Node*)0) { _Node* n = _M_node.allocate(1);
//   n->_M_next = n; n->_M_prev = n; _M_node._M_data = n; }` -- the proxy's null initialisation is the
//   `mov [esi+4],0` scheduled between the push and the malloc call; the sentinel is linked through the LOCAL before
//   it is published (writing through `_M_node._M_data` stores the member first and re-reads it).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

class CWorld;

class CMessageEvent {                                       // Ego_r.pdb, 0x58 bytes (only its size matters here)
public:
    unsigned char Data[0x58];
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x00
    _List_node_base* _M_prev;                               // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x08
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;   // +0x00: the sentinel node

    _List_base(const _Alloc& __a) : _M_node(_Node_allocator_type(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    explicit list(const _Alloc& __a = _Alloc()) : _List_base<_Tp, _Alloc>(__a) { }
};

} // namespace std

class CMessageEventManager {
public:
    CWorld*                     World;                      // +0x00
    std::list<CMessageEvent>   Messages;                   // +0x04
    CMessageEventManager(const CWorld& world);
};

CMessageEventManager::CMessageEventManager(const CWorld& world)
    : World((CWorld*)&world)
{
}