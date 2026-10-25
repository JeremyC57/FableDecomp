// std::vector::_M_insert_overflow(pointer position, const value_type& x, const __true_type&, size_type fill_len, bool atend) -- (vector<CUnprovenElement_004ac610*>) STLport POD growth insert: allocate size + max(size, fill_len) elements, memmove the head, write fill_len copies of x, memmove the tail unless atend, free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@PAVCUnprovenElement_004ac610@@V?$allocator@PAVCUnprovenElement_004ac610@@@std@@@std@@IAEX
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AC610, 216 bytes (+ int3 padding), thiscall, `ret 14h` (five arguments).
// Identity: element `CUnprovenElement_004ac610*` -- DESCRIPTIVE element type: `CUnprovenElement_004ac610*` is a placeholder for an unidentified 4-byte pointer element; evidence: the landed rough-area queries declare it over an invented `CThingMask47*` element; the real element is not named.
//   Family F007 (32 byte-identical bodies; the same STLport source as family F005, split from it only by the int3 padding):
//   the manifest label "Std_Vector_Insert_Range" is propagated and wrong (no range is inserted). STLport 4.6 `vector<_Tp>::_M_insert_overflow(pos, x,
//   const __true_type&, fill_len, atend)` (the protected POD overload, defined in the class body) verbatim.
// Callees: malloc 0x00BFEA0E (allocator::allocate), memmove 0x00BFEAE6 (__copy_trivial, head and tail), free 0x00BFEA14
//   (_M_clear's deallocate behind its null test).
// Shape: speed TU (the fill loop head aligned with `lea esp,[esp]`): `(max)` through its references (`lea ecx,[esp+20h]` /
//   `jb` / `lea ecx,[esp+0Ch]`), new_start homed in the dead fill_len slot and len in the pushed-ecx local, the fill
//   re-reading x through its reference each pass, then `_M_clear()` and `_M_set()`.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

class CUnprovenElement_004ac610;

namespace std {

struct __true_type { };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
                               : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
                                     + ((const char*)__last - (const char*)__first);
}

template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __value)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __value;
    return __first;
}

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear()
    {
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len,
                            bool __atend = false)
    {
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);

        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = (pointer)__copy_trivial(this->_M_start, __position, __new_start);
        // handle insertion
        __new_finish = fill_n(__new_finish, __fill_len, __x);
        if (!__atend)
            // copy remainder
            __new_finish = (pointer)__copy_trivial(__position, this->_M_finish, __new_finish);
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
};

template void vector<CUnprovenElement_004ac610* >::_M_insert_overflow(CUnprovenElement_004ac610**, CUnprovenElement_004ac610* const&, const __true_type&, unsigned int, bool);

} // namespace std