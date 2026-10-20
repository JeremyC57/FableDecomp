// CElement34Vector::operator=(const CElement34Vector&) -- STLport vector of a 52-byte non-POD element: copy assignment
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F6890, 288 bytes (incl. int3 padding); callers 0x009F54EF.
// The catalog label "CCombatWheel::operator=" is propagated; 0x34-byte element type unconfirmed. _M_allocate_and_copy (0x009F65E0) and __copy_ptrs (0x009F63B0) stay out of line here.
// Callees (LTCG register conventions, so declared __fastcall): __uninitialized_copy 0x009F65B0,
//   __copy_ptrs(first, last, result, const __false_type&) 0x009F63B0, _Destroy range 0x009F6720,
//   _M_clear 0x009F6850.
// Shape: STLport `vector::operator=` with `_M_allocate_and_copy` and `__copy_ptrs(first, last, result,
// const __false_type&)` both out of line in this TU. VC7.1 tail-duplicates the final
// `_M_finish = _M_start + n; return *this;` into every branch; the empty tag objects live in the dead
// argument slot. The tags need a user-declared empty constructor: value-initialising a POD empty struct
// (`__false_type()`) stores a zero byte and costs its own frame slot.
#include <stddef.h>

struct __false_type { __false_type() { } };

struct CElement34 {
    long m_word0;
    long m_word4;
    long m_word8;
    long m_wordc;
    long m_word10;
    long m_word14;
    long m_word18;
    long m_word1c;
    long m_word20;
    long m_word24;
    long m_word28;
    long m_word2c;
    long m_word30;
    CElement34& operator=(const CElement34& other);
};

CElement34* __fastcall UninitializedCopy(const CElement34* first, const CElement34* last,
                                                  CElement34* result, const __false_type&);                 // 0x009F65B0
CElement34* __fastcall CopyRange(const CElement34* first, const CElement34* last,
                                          CElement34* result, const __false_type&);    // 0x009F63B0
void __fastcall DestroyRange(CElement34* first, CElement34* last);                                // 0x009F6720

struct CElement34Vector {
    CElement34* _M_start;
    CElement34* _M_finish;
    CElement34* _M_end_of_storage;
    size_t size() const { return size_t(_M_finish - _M_start); }
    size_t capacity() const { return size_t(_M_end_of_storage - _M_start); }
    CElement34* _M_allocate_and_copy(size_t n, const CElement34* first, const CElement34* last);   // 0x009F65E0
    void _M_clear();                    // 0x009F6850
    CElement34Vector& operator=(const CElement34Vector& x);
};

static inline CElement34* CopyPtrs(const CElement34* first, const CElement34* last, CElement34* result)
{
    return CopyRange(first, last, result, __false_type());
}

CElement34Vector& CElement34Vector::operator=(const CElement34Vector& x)
{
    if (&x != this) {
        const size_t xlen = x.size();
        if (xlen > capacity()) {
            CElement34* tmp = _M_allocate_and_copy(xlen, x._M_start, x._M_finish);
            _M_clear();
            _M_start = tmp;
            _M_end_of_storage = _M_start + xlen;
        } else if (size() >= xlen) {
            CElement34* i = CopyPtrs(x._M_start, x._M_finish, _M_start);
            DestroyRange(i, _M_finish);
        } else {
            CopyPtrs(x._M_start, x._M_start + size(), _M_start);
            UninitializedCopy(x._M_start + size(), x._M_finish, _M_finish, __false_type());
        }
        _M_finish = _M_start + xlen;
    }
    return *this;
}
