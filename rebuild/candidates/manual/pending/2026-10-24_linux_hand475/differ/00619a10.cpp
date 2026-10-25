// std::_Rb_tree::_M_copy(_Rb_tree_node<value_type>* x, _Rb_tree_node<value_type>* p) -- map<CHeroLogBookEntry::ECategory, vector<CHeroLogBookEntry> > structural copy: clone x under p, recurse down every right child, walk the left spine
// Grade: DIFFER(301v299) -- frame only: retail spills __top into x's dead argument slot (`push ecx` frame), ours opens an 8-byte frame (`sub esp,8`) with __top in its own word; the loop-site inline vector copy is spelled with a forceinline twin constructor (plain STLport: both copies stay calls, 193v299)
// Leaf: ?_M_copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00619A10, 304 bytes (297 without padding), thiscall `ret 8`, returns the cloned top.
//   Callers: itself (0x00619A65, 0x00619B1A) and the tree copy constructor 0x00619C50 (`call` at 0x00619CAB).
// Callees: malloc 0x00BFEA0E (node and vector storage), vector<CHeroLogBookEntry>(const vector&) 0x0047BB90 (the top
//   clone's value copy, out of line), __uninitialized_copy<const CHeroLogBookEntry*, CHeroLogBookEntry*> 0x0047B710
//   (landed; the loop clone's inlined vector copy constructor), _M_copy itself.
// Identity: the node is 0x20 bytes (base 0x10 + a dword key + a 12-byte STLport vector) and the vector's element copy is
//   the landed CHeroLogBookEntry __uninitialized_copy: CHeroLogBook::LogEntries, Ego_r.pdb
//   map<CHeroLogBook::CHeroLogBookEntry::ECategory, vector<CHeroLogBook::CHeroLogBookEntry> >. The catalog label
//   "TreeNode_Clone" is descriptive.
// Shape: STLport 4.6 `_M_copy` verbatim with `_M_clone_node` / `_M_create_node` inline (null-tested placement copy of the
//   pair: the key dword, then the vector copy constructor: `_Vector_base(n, a)` zero stores, `n ? malloc(n * 12) : 0`,
//   then `_M_finish = __uninitialized_copy(..)`). Retail's inliner expanded the vector copy constructor at the loop's
//   clone but called it out of line (0x0047BB90) for the top clone; the lane's VC7.1 expands neither, so the loop clone
//   goes through `__forceinline` twins (`_M_clone_node_inline`, `vector(const vector&, int)`). `__false_type` needs its
//   user constructor (else `mov byte ptr [tag],0`).
extern "C" void* __cdecl malloc(unsigned int size);                                  // 0x00BFEA0E
inline void* operator new(unsigned int, void* __p) throw() { return __p; }

class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
    CWideString(const CWideString& other);                                           // 0x0099B720
};

class CHeroLogBook {
public:
    class CHeroLogBookEntry {
    public:
        enum ECategory { ECategory_Force32 = 0x7fffffff };
        CWideString Name;                                                            // +0x00
        CWideString AbbreviatedName;                                                 // +0x04
        CWideString Content;                                                         // +0x08
    };
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);   // 0x0047B710

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef const _Tp* const_pointer;
    typedef __false_type _IsPODType;
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    _Alloc get_allocator() const { return _Alloc(); }
    vector(const vector<_Tp, _Alloc>& __x);                                       // 0x0047BB90 (out of line)
    __forceinline vector(const vector<_Tp, _Alloc>& __x, int) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {                                                                             // the same body, expanded in place
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, this->_M_start, _IsPODType());
    }
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
    __forceinline pair(const pair<_T1, _T2>& __o, int) : first(__o.first), second(__o.second, 0) { }
};

struct _Rb_tree_node_base {
    bool                _M_color;                                                    // +0x00
    _Rb_tree_node_base* _M_parent;                                                   // +0x04
    _Rb_tree_node_base* _M_left;                                                     // +0x08
    _Rb_tree_node_base* _M_right;                                                    // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                                           // +0x10
};

template <class _Key, class _Value, class _Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value>, allocator<_Rb_tree_node<_Value> > > _M_header;
    unsigned int _M_node_count;

    static _Link_type& _S_left(_Rb_tree_node_base* __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Rb_tree_node_base* __x) { return (_Link_type&)(__x->_M_right); }
    _Link_type _M_create_node(const _Value& __x)
    {
        _Link_type __tmp = this->_M_header.allocate(1);
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
    __forceinline _Link_type _M_clone_node_inline(_Link_type __x)
    {
        _Link_type __tmp = this->_M_header.allocate(1);
        new (&__tmp->_M_value_field) _Value(__x->_M_value_field, 0);
        __tmp->_M_color = __x->_M_color;
        __tmp->_M_left = 0;
        __tmp->_M_right = 0;
        return __tmp;
    }
    _Link_type _M_clone_node(_Link_type __x)
    {
        _Link_type __tmp = _M_create_node(__x->_M_value_field);
        __tmp->_M_color = __x->_M_color;
        __tmp->_M_left = 0;
        __tmp->_M_right = 0;
        return __tmp;
    }
    _Link_type _M_copy(_Link_type __x, _Link_type __p);
};

template <class _Key, class _Value, class _Alloc>
typename _Rb_tree<_Key, _Value, _Alloc>::_Link_type _Rb_tree<_Key, _Value, _Alloc>::_M_copy(_Link_type __x, _Link_type __p)
{
    _Link_type __top = _M_clone_node(__x);
    __top->_M_parent = __p;

    if (__x->_M_right)
        __top->_M_right = _M_copy(_S_right(__x), __top);
    __p = __top;
    __x = _S_left(__x);

    while (__x != 0) {
        _Link_type __y = _M_clone_node_inline(__x);
        __p->_M_left = __y;
        __y->_M_parent = __p;
        if (__x->_M_right)
            __y->_M_right = _M_copy(_S_right(__x), __y);
        __p = __y;
        __x = _S_left(__x);
    }
    return __top;
}

typedef pair<const CHeroLogBook::CHeroLogBookEntry::ECategory, vector<CHeroLogBook::CHeroLogBookEntry> > CLogEntriesValue;
template class _Rb_tree<CHeroLogBook::CHeroLogBookEntry::ECategory, CLogEntriesValue, allocator<CLogEntriesValue> >;
}
