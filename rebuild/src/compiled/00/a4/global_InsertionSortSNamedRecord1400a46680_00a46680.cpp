// _global::InsertionSort_SNamedRecord14_00a46680(SNamedRecord14* first, SNamedRecord14* last, bool (__fastcall* comp)(const SNamedRecord14&, const SNamedRecord14&)) -- SGI/STLport __insertion_sort over 20-byte {CCharString, word, 12-byte record} records with a function-pointer comparator
// Leaf: InsertionSort_SNamedRecord14_00a46680
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A46680, length 102 bytes (+ int3 padding), `ret 4`.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30; __linear_insert(first, last, value, comp) @ 0x00A46530
// (catalog "Vector_InsertConditionalShift_Decal"; it calls `comp` through a register as `__fastcall`(&value, first), then
// the copy_backward @ 0x00A45910 and CCharString::operator=).
// Shape: `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, *i, comp);` -- /Gr
// algorithm TU (__fastcall: first in ecx -- saved in the frame word --, last in edx, the 4-byte comparator on the stack).
// The by-value element argument is built in the argument area by the record's implicit copy constructor (`push comp ;
// sub esp,14h ; mov esi,esp`, the out-of-line CCharString copy, then the word and the 12-byte record's memberwise user
// copy through the strength-reduced i+0x10 pointer); the callee destroys it. The same element layout as this chunk's
// __uninitialized_copy 0x00A458B0 (same TU).
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
    long        m_value;                         // +0x04
    SValue0C    m_record;                        // +0x08
};

typedef bool (__fastcall* SNamedRecordCompare)(const SNamedRecord14& a, const SNamedRecord14& b);

void __fastcall __linear_insert(SNamedRecord14* first, SNamedRecord14* last, SNamedRecord14 value, SNamedRecordCompare comp);   // 0x00A46530

void __fastcall InsertionSort_SNamedRecord14_00a46680(SNamedRecord14* first, SNamedRecord14* last, SNamedRecordCompare comp)
{
    if (first == last)
        return;
    for (SNamedRecord14* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}