// CElement8bVector::operator=(const CElement8bVector&) -- STLport vector of a 8-byte non-POD element: copy assignment
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00719B30, 256 bytes (incl. int3 padding); callers 0x00719CE8, 0x00719D09, 0x00719ECF.
// The catalog label "CCombatWheel::Assign" is propagated; byte twin (modulo callees) of 0x00568D40, 8-byte element type unconfirmed.
// Callees (LTCG register conventions, so declared __fastcall): __uninitialized_copy 0x004CF9D0,
//   __copy(first, last, result, const random_access_iterator_tag&, int*) 0x00718D10, _Destroy range 0x004D0EE0,
//   _M_clear 0x004D1460, malloc 0x00BFEA0E (STLport __malloc_alloc::allocate).
// Shape: STLport `vector::operator=` with `_M_allocate_and_copy` inlined (`n ? malloc(n*8) : 0`, then the
// out-of-line uninitialized copy); `__copy_ptrs(..., __false_type)` is inlined down to the __copy call with a
// default-constructed random_access_iterator_tag and a null distance pointer. VC7.1 tail-duplicates the final
// `_M_finish = _M_start + n; return *this;` into every branch; the empty tag objects live in the dead
// argument slot. The tags need a user-declared empty constructor: value-initialising a POD empty struct
// (`__false_type()`) stores a zero byte and costs its own frame slot.
#include <stddef.h>

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

struct CElement8b {
    long m_word0;
    long m_word4;
    CElement8b& operator=(const CElement8b& other);
};

extern "C" void* malloc(size_t size);   // 0x00BFEA0E

CElement8b* __fastcall UninitializedCopy(const CElement8b* first, const CElement8b* last,
                                                  CElement8b* result, const __false_type&);                 // 0x004CF9D0
CElement8b* __fastcall CopyRange(const CElement8b* first, const CElement8b* last,
                                          CElement8b* result, const random_access_iterator_tag&, int*);    // 0x00718D10
void __fastcall DestroyRange(CElement8b* first, CElement8b* last);                                // 0x004D0EE0

struct CElement8bVector {
    CElement8b* _M_start;
    CElement8b* _M_finish;
    CElement8b* _M_end_of_storage;
    size_t size() const { return size_t(_M_finish - _M_start); }
    size_t capacity() const { return size_t(_M_end_of_storage - _M_start); }
    CElement8b* allocate(size_t n) { return n != 0 ? (CElement8b*)malloc(n * sizeof(CElement8b)) : 0; }
    CElement8b* _M_allocate_and_copy(size_t n, const CElement8b* first, const CElement8b* last)
    {
        CElement8b* result = allocate(n);
        UninitializedCopy(first, last, result, __false_type());
        return result;
    }
    void _M_clear();                    // 0x004D1460
    CElement8bVector& operator=(const CElement8bVector& x);
};

static inline CElement8b* CopyPtrs(const CElement8b* first, const CElement8b* last, CElement8b* result)
{
    return CopyRange(first, last, result, random_access_iterator_tag(), (int*)0);
}

CElement8bVector& CElement8bVector::operator=(const CElement8bVector& x)
{
    if (&x != this) {
        const size_t xlen = x.size();
        if (xlen > capacity()) {
            CElement8b* tmp = _M_allocate_and_copy(xlen, x._M_start, x._M_finish);
            _M_clear();
            _M_start = tmp;
            _M_end_of_storage = _M_start + xlen;
        } else if (size() >= xlen) {
            CElement8b* i = CopyPtrs(x._M_start, x._M_finish, _M_start);
            DestroyRange(i, _M_finish);
        } else {
            CopyPtrs(x._M_start, x._M_start + size(), _M_start);
            UninitializedCopy(x._M_start + size(), x._M_finish, _M_finish, __false_type());
        }
        _M_finish = _M_start + xlen;
    }
    return *this;
}