// std::vector<CVertexGroupBlock>::operator=(const vector& x) -- STLport vector assignment: reallocate and copy-construct when x outgrows the capacity, else assign over the live blocks and destroy the surplus or copy-construct the extra tail
// Flags: /Gr
// Leaf: ??4?$vector@VCVertexGroupBlock@@V?$allocator@VCVertexGroupBlock@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADCA30, 347 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): Ego_r.pdb ??4?$vector@VCVertexGroupBlock@@V?$allocator@VCVertexGroupBlock@@@std@@@std@@QAEAAV01@ABV01@@Z;
//   0x18-byte elements (`imul 2AAAAAABh ; sar edx,2` = / 24) = Ego_r.pdb CVertexGroupBlock (Groups CArray<unsigned long>,
//   GroupToIndexMap map<unsigned long, long>; 12 + 12 bytes in retail STLport); its callees are the landed
//   __uninitialized_copy<const CVertexGroupBlock*, CVertexGroupBlock*> 0x00ADC440 (whose landed source already names this
//   caller), __destroy_aux<CVertexGroupBlock*> 0x00ADC190 (bound from the landed vector<CVertexGroupBlock>::reserve
//   0x00ADCB90) and __copy 0x00ADC120 (the element's implicit operator=: vector<unsigned long>::operator= 0x0045BC09 then the
//   map's 0x0045C79E). Ego_r.pdb CVertexGroupBlockOptimiser holds vector<CVertexGroupBlock> CurrentBlockSegmentGroups.
//   The catalog label "Vector_Reserve_Copy" is descriptive.
// Callees: malloc 0x00BFEA0E (allocate), free 0x00BFEA14 (deallocate), __uninitialized_copy 0x00ADC440, __destroy_aux
//   0x00ADC190, __copy<const CVertexGroupBlock*, CVertexGroupBlock*, int> 0x00ADC120 (all /Gr `__fastcall`).
// Shape: STLport 4.6 _vector.c operator= verbatim (`_M_allocate_and_copy` + `_M_clear` + the two `_M_start/_M_end_of_storage`
//   stores; `__copy_ptrs` + `_Destroy`; `__copy_ptrs` + `__uninitialized_copy`; then `_M_finish = _M_start + __xlen`), the
//   empty tags homed in the argument slot.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);   // 0x00ADC440
template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);                            // 0x00ADC190
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                   const random_access_iterator_tag&, _Distance*);                                                       // 0x00ADC120

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {                      // STLport allocator<T>: empty
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? static_cast<_Tp*>(malloc(__n * sizeof(_Tp))) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0
    _Tp* _M_finish;                                         // +4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +8
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    unsigned int capacity() const { return (unsigned int)(this->_M_end_of_storage._M_data - this->_M_start); }
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
protected:
    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(unsigned int __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
};

template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        const unsigned int __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
            _M_clear();
            this->_M_start = __tmp;
            this->_M_end_of_storage._M_data = this->_M_start + __xlen;
        }
        else if (size() >= __xlen) {
            pointer __i = __copy_ptrs((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, (pointer)this->_M_start, __false_type());
            _Destroy(__i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), (pointer)this->_M_start, __false_type());
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, this->_M_finish, __false_type());
        }
        this->_M_finish = this->_M_start + __xlen;
    }
    return *this;
}

template <class _Key, class _Tp, class _Compare, class _Alloc> class map {
public:
    void* _M_t[3];                                          // _Rb_tree: header, node count, compare (12 bytes)
};
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };

} // namespace std

template <class T> class CArray : public std::vector<T> { };

class CVertexGroupBlock {                                   // Ego_r.pdb, 0x18 bytes in retail
public:
    CArray<unsigned long>                                   Groups;            // +0x00
    std::map<unsigned long, long, std::less<unsigned long>,
             std::allocator<std::pair<const unsigned long, long> > > GroupToIndexMap;   // +0x0C
};

namespace std {
template class vector<CVertexGroupBlock>;
} // namespace std