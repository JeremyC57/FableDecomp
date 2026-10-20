// CElement8Vector::operator=(const CElement8Vector&) -- STLport vector of a 8-byte non-POD element: copy assignment
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00568D40, 242 bytes (incl. int3 padding); callers 0x0056388B.
// The catalog label "CCombatWheel::operator=" is propagated; the 8-byte element type is unconfirmed (callee labels are propagated: CSkeletalMorph / CSpeechFrame).
// Callees (LTCG register conventions, so declared __fastcall): __uninitialized_copy 0x005654B0,
//   __copy(first, last, result, const random_access_iterator_tag&, int*) 0x00565A90, _Destroy range 0x005684A0,
//   _M_clear 0x00568A90, malloc 0x00BFEA0E (STLport __malloc_alloc::allocate).
// Shape: STLport `vector::operator=` with `_M_allocate_and_copy` inlined (`n ? malloc(n*8) : 0`, then the
// out-of-line uninitialized copy); `__copy_ptrs(..., __false_type)` is inlined down to the __copy call with a
// default-constructed random_access_iterator_tag and a null distance pointer. VC7.1 tail-duplicates the final
// `_M_finish = _M_start + n; return *this;` into every branch; the empty tag objects live in the dead
// argument slot. The tags need a user-declared empty constructor: value-initialising a POD empty struct
// (`__false_type()`) stores a zero byte and costs its own frame slot.
#include <stddef.h>

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

struct CElement8 {
    long m_word0;
    long m_word4;
    CElement8& operator=(const CElement8& other);
};

extern "C" void* malloc(size_t size);   // 0x00BFEA0E

CElement8* __fastcall UninitializedCopy(const CElement8* first, const CElement8* last,
                                                  CElement8* result, const __false_type&);                 // 0x005654B0
CElement8* __fastcall CopyRange(const CElement8* first, const CElement8* last,
                                          CElement8* result, const random_access_iterator_tag&, int*);    // 0x00565A90
void __fastcall DestroyRange(CElement8* first, CElement8* last);                                // 0x005684A0

struct CElement8Vector {
    CElement8* _M_start;
    CElement8* _M_finish;
    CElement8* _M_end_of_storage;
    size_t size() const { return size_t(_M_finish - _M_start); }
    size_t capacity() const { return size_t(_M_end_of_storage - _M_start); }
    CElement8* allocate(size_t n) { return n != 0 ? (CElement8*)malloc(n * sizeof(CElement8)) : 0; }
    CElement8* _M_allocate_and_copy(size_t n, const CElement8* first, const CElement8* last)
    {
        CElement8* result = allocate(n);
        UninitializedCopy(first, last, result, __false_type());
        return result;
    }
    void _M_clear();                    // 0x00568A90
    CElement8Vector& operator=(const CElement8Vector& x);
};

static inline CElement8* CopyPtrs(const CElement8* first, const CElement8* last, CElement8* result)
{
    return CopyRange(first, last, result, random_access_iterator_tag(), (int*)0);
}

CElement8Vector& CElement8Vector::operator=(const CElement8Vector& x)
{
    if (&x != this) {
        const size_t xlen = x.size();
        if (xlen > capacity()) {
            CElement8* tmp = _M_allocate_and_copy(xlen, x._M_start, x._M_finish);
            _M_clear();
            _M_start = tmp;
            _M_end_of_storage = _M_start + xlen;
        } else if (size() >= xlen) {
            CElement8* i = CopyPtrs(x._M_start, x._M_finish, _M_start);
            DestroyRange(i, _M_finish);
        } else {
            CopyPtrs(x._M_start, x._M_start + size(), _M_start);
            UninitializedCopy(x._M_start + size(), x._M_finish, _M_finish, __false_type());
        }
        _M_finish = _M_start + xlen;
    }
    return *this;
}
