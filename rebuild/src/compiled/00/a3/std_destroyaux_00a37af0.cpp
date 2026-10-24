// std::__destroy_aux(SListRecord* first, SListRecord* last, const __false_type&) -- destroy a range of 20-byte {list<POD>, three words, CCharString} records
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A37AF0, length 88 bytes (padded to 96).
// Leaf: __destroy_aux
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. The stride is 0x14; the
// inlined element destructor destroys a CCharString at +0x10 and then an STLport list at +0x00 (inline clear(): walk
// from the sentinel's next, advance before freeing each node, re-point the sentinel at itself, then free the sentinel
// under a null test). The list holds a trivially destructible type (no per-node destructor). Nine callers
// (0x00A3737D CTCInventoryItem::OnDie area ...); the record class is not identified, so it is modelled as SListRecord.
// Convention: LTCG register form -- first in ecx, last in edx, the tag pointer on the stack (`ret 4`).
// Callees: CCharString::~CCharString @ 0x0099EAE0, free @ 0x00BFEA14 (nodes and sentinel).
// Shape: the STLport _List_base (sentinel in an _STLP_alloc_proxy) with clear() inline in ~_List_base; the sentinel
// member is re-read after every free (the call may alias it).
extern "C" void __cdecl free(void* p);      // 0x00BFEA14
struct CCharString {
    void* m_pData;
    ~CCharString();                         // 0x0099EAE0
};
namespace std {
struct __false_type { };
template <class T> inline void _Destroy(T* p) { p->~T(); }
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template <class T> struct allocator {
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
};
template <class T> struct _List_base {
    _STLP_alloc_proxy<_List_node<T>*, allocator<_List_node<T> > > _M_node;
    void clear()
    {
        _List_node<T>* cur = (_List_node<T>*)_M_node._M_data->_M_next;
        while (cur != _M_node._M_data) {
            _List_node<T>* tmp = cur;
            cur = (_List_node<T>*)cur->_M_next;
            _M_node.deallocate(tmp, 1);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
};
template <class T> struct list : _List_base<T> { };
}
struct SListRecord {                        // 20-byte element
    std::list<long> m_items;                // +0x00
    long m_values[3];                       // +0x04
    CCharString m_name;                     // +0x10
};
namespace std {
void __fastcall __destroy_aux(SListRecord* first, SListRecord* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
}