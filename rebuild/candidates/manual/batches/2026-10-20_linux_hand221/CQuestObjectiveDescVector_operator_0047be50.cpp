// CQuestObjectiveDescVector::operator=(const CQuestObjectiveDescVector&) -- STLport vector of a 12-byte non-POD element: copy assignment
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047BE50, 320 bytes (incl. int3 padding); callers 0x0047C3B5,
// 0x0047C63F. The catalog label "CCombatWheel::operator=" is propagated (0x0047BD70, called here as the
// vector's _M_clear, carries the same propagated CCombatWheel name); the element is 12 bytes with a
// non-trivial assignment -- the manifest's propagated callee labels suggest CHeroLogBook::CQuestObjectiveDesc.
// Callees (LTCG register conventions, so declared __fastcall): __uninitialized_copy 0x0047B830,
//   __copy(first, last, result, const random_access_iterator_tag&, int*) 0x0047B8E0, _Destroy range 0x0047BCA0,
//   _M_clear 0x0047BD70, malloc 0x00BFEA0E (STLport __malloc_alloc::allocate).
// Shape: STLport `vector::operator=` with `_M_allocate_and_copy` inlined (`n ? malloc(n*12) : 0`, then the
// out-of-line uninitialized copy); `__copy_ptrs(..., __false_type)` is inlined down to the __copy call with a
// default-constructed random_access_iterator_tag and a null distance pointer. VC7.1 tail-duplicates the final
// `_M_finish = _M_start + n; return *this;` into every branch; the empty tag objects live in the dead
// argument slot. The tags need a user-declared empty constructor: value-initialising a POD empty struct
// (`__false_type()`) stores a zero byte and costs its own frame slot.
#include <stddef.h>

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

struct CQuestObjectiveDesc {
    long m_word0;
    long m_word4;
    long m_word8;
    CQuestObjectiveDesc& operator=(const CQuestObjectiveDesc& other);
};

extern "C" void* malloc(size_t size);   // 0x00BFEA0E

CQuestObjectiveDesc* __fastcall UninitializedCopy(const CQuestObjectiveDesc* first, const CQuestObjectiveDesc* last,
                                                  CQuestObjectiveDesc* result, const __false_type&);                 // 0x0047B830
CQuestObjectiveDesc* __fastcall CopyRange(const CQuestObjectiveDesc* first, const CQuestObjectiveDesc* last,
                                          CQuestObjectiveDesc* result, const random_access_iterator_tag&, int*);    // 0x0047B8E0
void __fastcall DestroyRange(CQuestObjectiveDesc* first, CQuestObjectiveDesc* last);                                // 0x0047BCA0

struct CQuestObjectiveDescVector {
    CQuestObjectiveDesc* _M_start;
    CQuestObjectiveDesc* _M_finish;
    CQuestObjectiveDesc* _M_end_of_storage;
    size_t size() const { return size_t(_M_finish - _M_start); }
    size_t capacity() const { return size_t(_M_end_of_storage - _M_start); }
    CQuestObjectiveDesc* allocate(size_t n) { return n != 0 ? (CQuestObjectiveDesc*)malloc(n * sizeof(CQuestObjectiveDesc)) : 0; }
    CQuestObjectiveDesc* _M_allocate_and_copy(size_t n, const CQuestObjectiveDesc* first, const CQuestObjectiveDesc* last)
    {
        CQuestObjectiveDesc* result = allocate(n);
        UninitializedCopy(first, last, result, __false_type());
        return result;
    }
    void _M_clear();                    // 0x0047BD70
    CQuestObjectiveDescVector& operator=(const CQuestObjectiveDescVector& x);
};

static inline CQuestObjectiveDesc* CopyPtrs(const CQuestObjectiveDesc* first, const CQuestObjectiveDesc* last, CQuestObjectiveDesc* result)
{
    return CopyRange(first, last, result, random_access_iterator_tag(), (int*)0);
}

CQuestObjectiveDescVector& CQuestObjectiveDescVector::operator=(const CQuestObjectiveDescVector& x)
{
    if (&x != this) {
        const size_t xlen = x.size();
        if (xlen > capacity()) {
            CQuestObjectiveDesc* tmp = _M_allocate_and_copy(xlen, x._M_start, x._M_finish);
            _M_clear();
            _M_start = tmp;
            _M_end_of_storage = _M_start + xlen;
        } else if (size() >= xlen) {
            CQuestObjectiveDesc* i = CopyPtrs(x._M_start, x._M_finish, _M_start);
            DestroyRange(i, _M_finish);
        } else {
            CopyPtrs(x._M_start, x._M_start + size(), _M_start);
            UninitializedCopy(x._M_start + size(), x._M_finish, _M_finish, __false_type());
        }
        _M_finish = _M_start + xlen;
    }
    return *this;
}
