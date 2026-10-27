// CEngineManager::AddEngine(CIEngine& engine) -- register it: append its address to the Engines list
// Leaf: ?AddEngine@CEngineManager@@QAEXAAVCIEngine@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006404D0, 44 bytes, thiscall, `ret 4`.
// Identity (proven): its caller CDisplayEngine::InitEngine 0x00435190 passes CTBaseSingleton<CEngineManager>::Get() 0x00436FB0 as `this` and the engine it just created; the list at +4 is Ego_r.pdb CEngineManager::Engines (list<CIEngine*>); Ego_r.pdb ?AddEngine@CEngineManager@@QAEXAAVCIEngine@@@Z.
// Callees: malloc 0x00BFEA0E (the 12-byte node).
// Shape (family M088, representative 0x004BD390): `Engines.push_back(&engine);` -- the landed list push_back 0x004BA420 model
//   inline (allocate(1), the placement store under its null test, the four link stores); speed TU.
// Header debt: the class is modelled locally by the list member's retail offset (the generated header flattens the list).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CIEngine;
namespace std {
struct _List_node_base {
    _List_node_base* _M_next;                             // +0x0
    _List_node_base* _M_prev;                             // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                          // +0x8
};
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> struct allocator {
    __forceinline _Tp* allocate(unsigned int __n) { return static_cast<_Tp*>(malloc(__n * sizeof(_Tp))); }
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    __forceinline _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    __forceinline _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    struct : allocator<_Node> { _List_node_base* _M_data; } _M_node;   // _STLP_alloc_proxy: the sentinel
    __forceinline iterator end() { return iterator(_M_node._M_data); }
    __forceinline _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = _M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    __forceinline iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    __forceinline void push_back(const _Tp& __x) { insert(end(), __x); }
};
}
class CEngineManager {
public:
    void AddEngine(CIEngine& engine);
    unsigned char _pad[0x4];                    // +0x0 .. members before Engines
    std::list<CIEngine*> Engines;              // +0x4
};
void CEngineManager::AddEngine(CIEngine& engine)
{
    Engines.push_back(&engine);
}