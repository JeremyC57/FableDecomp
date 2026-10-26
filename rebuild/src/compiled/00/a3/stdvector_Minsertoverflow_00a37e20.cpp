// std::vector::_M_insert_overflow(pointer position, const value_type& x, const __false_type&, size_type fill_len, bool atend) -- (vector<CClassTypeInfo >) STLport growth insert: allocate size + max(size, fill_len) elements, copy the head, x (once, or fill_len times) and the tail unless atend, destroy and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@VCClassTypeInfo@@V?$allocator@VCClassTypeInfo@@@std@@@std@@IAEXPAVCClassTypeInfo@@ABV3@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A37E20, 286 bytes (+ int3 padding), thiscall, `ret 14h` (five arguments).
// Identity (proven): std::vector<CClassTypeInfo >::_M_insert_overflow; evidence: its only caller is vector<CClassTypeInfo>::_M_fill_insert 0x00A37F40 (0x00A3806A), the fill-insert of the landed vector<CClassTypeInfo>::resize(n, x) 0x00A380E0 / resize(n) 0x00A38150 chain (CClassFactory::ClassTypeInfo); the element copy is the landed CClassTypeInfo copy constructor 0x00A37B50 and the destroy the landed __destroy_aux<CClassTypeInfo*> 0x00A37AF0; the 20-byte stride (0x66666667 reciprocal, `sar edx,3`) is retail's CClassTypeInfo.
// Label: the manifest's "Vector_Reallocate_CFeatDef" is propagated from a byte-identical body elsewhere (family J085: the 286-byte growth inserts
//   of 12/20-byte class elements; retail has no identical-code folding, so each address is its own element type).
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14; the element copy CClassTypeInfo::CClassTypeInfo(const CClassTypeInfo&) 0x00A37B50 (landed, ??0CClassTypeInfo@@QAE@ABV0@@Z) (thiscall, `ret 4`; head, x and fill copies);
//   __uninitialized_copy<CClassTypeInfo*, CClassTypeInfo*>(first, last, result, const __false_type&) 0x00A37D50 (the tail, __fastcall, `ret 8`); __destroy_aux<CClassTypeInfo*>(first, last, const __false_type&) 0x00A37AF0 (landed, ??$__destroy_aux@PAVCClassTypeInfo@@@std@@YIXPAVCClassTypeInfo@@0ABU__false_type@0@@Z) (_M_clear's _Destroy, __fastcall, `ret 4`).
// Shape: STLport 4.6 vector.c verbatim (no EH): the head copy and the fill are inline loops around the out-of-line element copy
//   (placement new's null test), the tail copy stays out of line, and every __false_type tag shares the live atend slot (an
//   empty class with a user constructor).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    vector(const vector<_Tp, _Alloc>& __x);               // out of line
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
} // namespace std
namespace std {
template <class _Tp> struct _List_node;
template <class _Tp, class _Alloc = allocator<_Tp> >
class list {                                               // 4 bytes in retail
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;   // +0x00
};
}
class CClass;
class CCharString {                                        // 4 bytes (TLC)
public:
    void* PStringData;                                     // +0x00
};
class CClassTypeInfo {                                     // 20 bytes in retail (28 in Ego_r.pdb)
public:
    CClassTypeInfo(const CClassTypeInfo& other);           // 0x00A37B50 (out of line)
    std::list<CClass*> TypeList;                           // +0x00
    unsigned long NoAllocated;                             // +0x04
    bool Used;                                             // +0x08
    bool Local;                                            // +0x09
    CClass* (__cdecl* AllocFunc)();                        // +0x0C
    CCharString Name;                                      // +0x10
};
typedef CClassTypeInfo TElement_00a37e20;
template void std::vector<TElement_00a37e20 >::_M_insert_overflow(TElement_00a37e20*, const TElement_00a37e20&, const std::__false_type&,
                                                               unsigned int, bool);