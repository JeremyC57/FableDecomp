// CElement44Vector::operator=(const CElement44Vector&) -- STLport vector of a 68-byte non-POD element: copy assignment
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071A160, 276 bytes (incl. int3 padding); callers 0x00717A78 .. 0x0071A288 (8 sites).
// The catalog label "CCombatWheel::CopyAssign" is propagated; 0x44-byte element type unconfirmed. _M_allocate_and_copy is out of line here (0x00719FA0).
// Callees (LTCG register conventions, so declared __fastcall): __uninitialized_copy 0x00719E20,
//   __copy(first, last, result, const random_access_iterator_tag&, int*) 0x00719F00, _Destroy range 0x004D26F0,
//   _M_clear 0x004D2720.
// Shape: STLport `vector::operator=` (`_M_allocate_and_copy` stays out of line for this element); `__copy_ptrs(..., __false_type)` is inlined down to the __copy call with a
// default-constructed random_access_iterator_tag and a null distance pointer. VC7.1 tail-duplicates the final
// `_M_finish = _M_start + n; return *this;` into every branch; the empty tag objects live in the dead
// argument slot. The tags need a user-declared empty constructor: value-initialising a POD empty struct
// (`__false_type()`) stores a zero byte and costs its own frame slot.
#include <stddef.h>

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

struct CElement44 {
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
    long m_word34;
    long m_word38;
    long m_word3c;
    long m_word40;
    CElement44& operator=(const CElement44& other);
};

CElement44* __fastcall UninitializedCopy(const CElement44* first, const CElement44* last,
                                                  CElement44* result, const __false_type&);                 // 0x00719E20
CElement44* __fastcall CopyRange(const CElement44* first, const CElement44* last,
                                          CElement44* result, const random_access_iterator_tag&, int*);    // 0x00719F00
void __fastcall DestroyRange(CElement44* first, CElement44* last);                                // 0x004D26F0

struct CElement44Vector {
    CElement44* _M_start;
    CElement44* _M_finish;
    CElement44* _M_end_of_storage;
    size_t size() const { return size_t(_M_finish - _M_start); }
    size_t capacity() const { return size_t(_M_end_of_storage - _M_start); }
    CElement44* _M_allocate_and_copy(size_t n, const CElement44* first, const CElement44* last);   // 0x00719FA0
    void _M_clear();                    // 0x004D2720
    CElement44Vector& operator=(const CElement44Vector& x);
};

static inline CElement44* CopyPtrs(const CElement44* first, const CElement44* last, CElement44* result)
{
    return CopyRange(first, last, result, random_access_iterator_tag(), (int*)0);
}

CElement44Vector& CElement44Vector::operator=(const CElement44Vector& x)
{
    if (&x != this) {
        const size_t xlen = x.size();
        if (xlen > capacity()) {
            CElement44* tmp = _M_allocate_and_copy(xlen, x._M_start, x._M_finish);
            _M_clear();
            _M_start = tmp;
            _M_end_of_storage = _M_start + xlen;
        } else if (size() >= xlen) {
            CElement44* i = CopyPtrs(x._M_start, x._M_finish, _M_start);
            DestroyRange(i, _M_finish);
        } else {
            CopyPtrs(x._M_start, x._M_start + size(), _M_start);
            UninitializedCopy(x._M_start + size(), x._M_finish, _M_finish, __false_type());
        }
        _M_finish = _M_start + xlen;
    }
    return *this;
}