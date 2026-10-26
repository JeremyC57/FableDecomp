// _global::InsertionSort_SNamedRecord14_00a717a0(SNamedRecord14* first, SNamedRecord14* last, SNamedRecordCompare comp) -- SGI/STLport __insertion_sort over 20-byte {CCharString, 12-byte record, word} records ordered by name (empty comparator)
// Leaf: InsertionSort_SNamedRecord14_00a717a0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A717A0, length 102 bytes (+ int3 padding), `ret 4`.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30; __linear_insert(first, last, value, comp) @ 0x00A70F40
// (catalog "Vector_ConstructElement_Pair"; it inlines the comparator as CCharString::operator< @ 0x009E6530 on the names).
// Shape: `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, *i, comp);` -- /Gr
// algorithm TU (__fastcall: first in ecx -- saved in the frame word --, last in edx, the empty comparator's 4-byte slot on
// the stack, re-pushed as a dword).
// The by-value element argument is built in the argument area by the record's implicit copy constructor (`push comp ;
// sub esp,14h ; mov esi,esp`, the out-of-line CCharString copy, then the 12-byte record's memberwise user copy through
// the strength-reduced i+0xC pointer and the word at +0x10); the callee destroys it. The same element layout as this
// chunk's __uninitialized_copy 0x00A6ED80 (the record before the word moves the pointer from +0x10 to +0xC, cf. 0x00A46680).
// Label correction: the catalog's `_Copy_impl<NPlayerTargeting::CTargetPoint*,...>` is propagated.
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);       // 0x0099EC30
};

struct SValue0C {                                // 12-byte value with a memberwise user copy constructor
    long m_word0, m_word4, m_word8;
    SValue0C(const SValue0C& other) : m_word0(other.m_word0), m_word4(other.m_word4), m_word8(other.m_word8) { }
};

struct SNamedRecord14 {                          // 20 bytes
    CCharString m_name;                          // +0x00
    SValue0C    m_record;                        // +0x04
    long        m_value;                         // +0x10
};

struct SNamedRecordCompare {                     // empty functor ordering by name (the callee inlines CCharString::operator<)
    bool operator()(const SNamedRecord14& a, const SNamedRecord14& b) const;
};

void __fastcall __linear_insert(SNamedRecord14* first, SNamedRecord14* last, SNamedRecord14 value, SNamedRecordCompare comp);   // 0x00A70F40

void __fastcall InsertionSort_SNamedRecord14_00a717a0(SNamedRecord14* first, SNamedRecord14* last, SNamedRecordCompare comp)
{
    if (first == last)
        return;
    for (SNamedRecord14* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}