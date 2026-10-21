// std::vector<CActionInputControl>::operator=(const vector&) -- STLport vector assignment for a trivially copyable 28-byte element: reallocate and copy-construct when the source exceeds the capacity, else assign over the live elements, or assign the overlap and copy-construct the rest
// Leaf: ??4
// Retail 0x00719540, 336 bytes, `ret 4`. Label: catalogued as CSubtitledMessage::Assign_CActionInputControl (the vector is a
// CSubtitledMessage member); the body is the STLport 4.x vector<T>::operator= instantiation (sibling of the landed
// vector<CCombatWheel::SSlot>::operator= 0x006F3C20).
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14 (_M_clear: trivial destroy + deallocate inline),
// __uninitialized_copy(first, last, result, const __false_type&) 0x004CFAC0 and __copy(first, last, result,
// const random_access_iterator_tag&, ptrdiff_t*) 0x00718530 (both `__fastcall`, labelled _Uninit_copy / _Copy_impl).
// Shape: the STLport text; the shrinking arm has no _Destroy call (trivial element); same-TU stand-ins for the two copy
// helpers (auto_inline(off), above the caller) keep the tag temporaries in the dead argument slot and push the null
// distance pointer once for both arms.
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

struct CActionInputControl {                               // 0x1c bytes, trivially copyable
    long          Action;                                  // +0x00
    unsigned long Data[6];                                 // +0x04
};

#pragma auto_inline(off)
CActionInputControl* __fastcall __uninitialized_copy(const CActionInputControl* first, const CActionInputControl* last,
                                                     CActionInputControl* result, const __false_type&)   // 0x004CFAC0
{
    for (; first != last; ++first, ++result)
        *result = *first;
    return result;
}
CActionInputControl* __fastcall __copy(const CActionInputControl* first, const CActionInputControl* last,
                                       CActionInputControl* result, const random_access_iterator_tag&, int*)   // 0x00718530
{
    for (int n = int(last - first); n > 0; --n, ++first, ++result)
        *result = *first;
    return result;
}
#pragma auto_inline(on)
inline CActionInputControl* __copy_ptrs(const CActionInputControl* first, const CActionInputControl* last,
                                        CActionInputControl* result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}

struct CActionInputControlVector {                         // STLport vector<CActionInputControl> (12 bytes)
    typedef unsigned int size_type;
    CActionInputControl* _M_start;
    CActionInputControl* _M_finish;
    CActionInputControl* _M_end_of_storage;
    size_type size() const { return size_type(_M_finish - _M_start); }
    size_type capacity() const { return size_type(_M_end_of_storage - _M_start); }
    CActionInputControl* allocate(size_type n) { return n != 0 ? (CActionInputControl*)malloc(n * sizeof(CActionInputControl)) : 0; }
    CActionInputControl* _M_allocate_and_copy(size_type n, const CActionInputControl* first, const CActionInputControl* last)
    {
        CActionInputControl* result = allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
    void _M_clear()
    {
        if (_M_start)
            free(_M_start);
    }
    CActionInputControlVector& operator=(const CActionInputControlVector& x);
};

CActionInputControlVector& CActionInputControlVector::operator=(const CActionInputControlVector& x)
{
    if (&x != this) {
        const size_type xlen = x.size();
        if (xlen > capacity()) {
            CActionInputControl* tmp = _M_allocate_and_copy(xlen, x._M_start, x._M_finish);
            _M_clear();
            _M_start = tmp;
            _M_end_of_storage = _M_start + xlen;
        } else if (size() >= xlen) {
            __copy_ptrs(x._M_start, x._M_finish, _M_start, __false_type());
        } else {
            __copy_ptrs(x._M_start, x._M_start + size(), _M_start, __false_type());
            __uninitialized_copy(x._M_start + size(), x._M_finish, _M_finish, __false_type());
        }
        _M_finish = _M_start + xlen;
    }
    return *this;
}