// std::vector<CMinimapData::MinimapTheme>::vector(const vector& x) -- STLport vector copy: allocate x.size() 4-byte elements (none for an empty source), set start/finish/end of storage, placement-copy the elements
// Leaf: ??0?$vector@W4MinimapTheme@CMinimapData@@V?$allocator@W4MinimapTheme@CMinimapData@@@std@@@std@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082C620, 112 bytes, thiscall, `ret 4` (&x), returns this.
// Identity (probable): the landed __uninitialized_copy<const CMinimapData*> 0x0082CB70 copies each record's MinimapThemeList (Ego_r.pdb vector<MinimapTheme> at retail +8) through it; reloc_bindings has this name; the other caller 0x0082CAC0 is the CMinimapData copy constructor.
// Callees: malloc 0x00BFEA0E.
// Shape (family K081, representative 0x00419F50): STLport 4.6 `vector(const _Self& __x) : _Vector_base(__x.size(),
//   __x.get_allocator()) { _M_finish = __uninitialized_copy(__x._M_start, __x._M_finish, _M_start, _IsPODType()); }`
//   with the __false_type copy inlined (the element has no __type_traits specialisation): placement-new null test per element.
//   For the CConsoleCommandParameters member copy the vector constructor is __forceinline (VC7.1 otherwise calls it).
extern "C" void* __cdecl malloc(unsigned int size);                       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
class CMinimapData {
public:
    enum MinimapTheme { };                                // Ego_r.pdb (enumerators not needed)
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {                                    // STLport allocator over the malloc allocator: empty
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
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
    _Tp* _M_start;                                                        // +0x0
    _Tp* _M_finish;                                                       // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;               // +0x8
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef const _Tp* const_pointer;
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    vector(const vector& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish, this->_M_start,
                                               __false_type());
    }
};
}
template std::vector<CMinimapData::MinimapTheme>::vector(const std::vector<CMinimapData::MinimapTheme>&);