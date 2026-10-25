// std::vector::vector(const vector& __x) -- (vector<CHeroLogBook::CHeroLogBookEntry >) STLport copy constructor: allocate size() elements, uninitialized-copy the source into them
// Leaf: ??0?$vector@VCHeroLogBookEntry@CHeroLogBook@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047BB90, 103 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: std::vector<CHeroLogBook::CHeroLogBookEntry >::vector(const vector&) -- proven; evidence: its callers copy pair<const CHeroLogBookEntry::ECategory, vector<CHeroLogBookEntry> > values (key dword at +0, this vector at +4: 0x0047BD50, 0x0047BE30, the map node clones 0x0047C500 / 0x00619950 / 0x00619A10) = the Ego_r CHeroLogBook::LogEntries map's mapped type, and CHeroLogBook_AddLogEntry 0x0047A9F0 / 0x0047CB50; its element copy 0x0047B710 runs CWideString::CWideString(const CWideString&) 0x0099B720 on +0, +4, +8 = Ego_r CHeroLogBookEntry {Name, AbbreviatedName, Content}.
//   Family G068 (STLport vector copy constructors of 12/20/24-byte class elements; catalogue label "std::vector_copy_constructor" is
//   propagated/behavioural).
// Callees: malloc 0x00BFEA0E (allocator::allocate), __uninitialized_copy<const T*, T*>(first, last, result, const
//   __false_type&) 0x0047B710 (out of line, __fastcall, `ret 8`).
// Shape: STLport 4.6 verbatim: _Vector_base(__x.size(), __x.get_allocator()) zero-initialises the three pointers before the
//   allocation; the __false_type tag (_IsPODType of a class element) is homed in the dead __x argument slot.
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
class CWideString { public: void* PStringData; };     // 4 bytes
class CHeroLogBook {
public:
    class CHeroLogBookEntry {                              // 12 bytes (Ego_r.pdb)
    public:
        CWideString Name;                                  // +0x00
        CWideString AbbreviatedName;                       // +0x04
        CWideString Content;                               // +0x08
    };
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
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
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                         // +0
    _Tp* _M_finish;                                        // +4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +8
};
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef const _Tp* const_pointer;
    typedef _Alloc allocator_type;
    typedef __false_type _IsPODType;                       // __type_traits<_Tp>::is_POD_type of a class element
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return allocator_type(this->_M_end_of_storage); }
    vector(const vector<_Tp, _Alloc>& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, this->_M_start, _IsPODType());
    }
};
template vector<CHeroLogBook::CHeroLogBookEntry >::vector(const vector<CHeroLogBook::CHeroLogBookEntry >&);
}