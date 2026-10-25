// std::list::list(const allocator_type& a) -- STLport list<CFileInstaller::CActiveFile> constructor: null the node word, then malloc a 0xB8-byte sentinel linked to itself
// Leaf: ??0?$list@VCActiveFile@CFileInstaller@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7730, 35 bytes (+ int3 padding), thiscall `ret 4` (the allocator
// reference), returns this.
// Identity: the 0xB8-byte node is list<CFileInstaller::CActiveFile>: the neighbouring `_M_create_node` 0x009C7790
// mallocs 0xB8 bytes and copy-constructs the element at +8 through CActiveFile::CActiveFile(const CActiveFile&)
// @ 0x009C72B0 (landed: retail CActiveFile is 0xB0 bytes). FableWin.pdb has the list<CActiveFile> default ctor.
// No direct callers (inlined everywhere else; an emitted COMDAT). Label correction: `ListSentinel_Ctor_szb8_009c7730`.
// Callee: malloc @ 0x00BFEA0E (allocator::allocate(1)).
// Shape: STLport `_List_base(const allocator_type&)`: the `_STLP_alloc_proxy` stores the null node word first
// (scheduled after the push), the sentinel is built in a local (self-linked through eax) and stored last, as in
// the landed CDelayedAnimation ctor 0x00711D50; `list(const allocator_type& = allocator_type())` gives `ret 4`.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CFileInstaller {
public:
    class CActiveFile {
    public:
        unsigned long _body[0xB0 / 4];                  // retail 0xB0 bytes (landed copy ctor 0x009C72B0)
    };
};

namespace std {

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

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _Alloc allocator_type;
    _STLP_alloc_proxy<_List_node<_Tp>*, allocator<_List_node<_Tp> > > _M_node;
    _List_base(const allocator_type&) : _M_node(allocator<_List_node<_Tp> >(), (_List_node<_Tp>*)0)
    {
        _List_node<_Tp>* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit list(const allocator_type& __a = allocator_type());
};

template <class _Tp, class _Alloc> list<_Tp, _Alloc>::list(const allocator_type& __a)
    : _List_base<_Tp, _Alloc>(__a)
{
}

template class list<CFileInstaller::CActiveFile>;

}