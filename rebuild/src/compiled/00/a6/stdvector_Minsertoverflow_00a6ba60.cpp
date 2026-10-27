// std::vector::_M_insert_overflow(CIVCountedPointer<CManagedTextureData>* __position, const CIVCountedPointer<CManagedTextureData>& __x, const __false_type&, unsigned int __fill_len, bool __atend) -- (vector<CIVCountedPointer<CManagedTextureData> >) STLport grow path: allocate size + max(size, fill_len), copy the head, construct fill_len copies of x, copy the tail unless atend, destroy and free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@V?$CIVCountedPointer@VCManagedTextureData@@@@V?$allocator@V?$CIVCountedPointer@VCManagedTextureData@@@@@std@@@std@@QAEXPAV?$CIVCountedPointer@VCManagedTextureData@@@@ABV3@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6BA60, 245 bytes, thiscall, `ret 14h`.
// Identity (proven): its __destroy_aux 0x00A6B700 is bound ??$__destroy_aux@PAV?$CIVCountedPointer@VCManagedTextureData@@@@@std@@...; its only caller is that vector's _M_fill_insert 0x00A6BCD0.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy<T*, T*>(first, last, result, const __false_type&) 0x00A6AFD0 (the tail),
//   __destroy_aux<T*>(first, last, const __false_type&) 0x00A6B700, free 0x00BFEA14.
// Shape (family M198, representative 0x0069DBB0): STLport 4.6 `_M_insert_overflow(pos, x, const __false_type&, fill_len, atend)`
//   verbatim with plain `inline` helpers: VC7.1 inlines the head __uninitialized_copy and leaves the tail one out of line
//   (the landed CGraphicInfo note 0x00A01120), the smart pointer's add-ref copy (`if (p) ++p->count`, count at +4) inline,
//   _M_clear's _Destroy -> __destroy -> __destroy_aux out of line; the tags are homed in the dead atend slot. /Gr TU.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
class CIVCountedPointeeBase {
public:
    virtual void Destroy(int flags);                      // slot 0
    virtual void Delete();                                // slot 1
    unsigned long IVRefCount;                             // +0x04
};
template <class T> class CIVCountedPointer {               // Ego_r.pdb, 4 bytes
public:
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    ~CIVCountedPointer();
    CIVCountedPointeeBase* Object;                        // +0x00
};
class CManagedTextureData;
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00A6B700
template <class _ForwardIterator, class _Tp>
inline void __fastcall __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _ForwardIterator> inline void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, __first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
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
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false);
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
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
template void vector<CIVCountedPointer<CManagedTextureData> >::_M_insert_overflow(CIVCountedPointer<CManagedTextureData>*, const CIVCountedPointer<CManagedTextureData>&, const __false_type&, unsigned int, bool);
}