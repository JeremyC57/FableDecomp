// CEntryVector_00bef9c0::push_back(const SEntry_00befbc0& entry) -- append a 28-byte {key, value} record: copy-construct it in place when there is room, else hand it to the reallocating insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEF9C0, length 87 bytes (`ret 4`). Called from the landed
// CEntryTable_00befbc0::AddEntry @ 0x00BEFBC0. Callees: the record's vector copy constructor @ 0x005BB346, the vector's
// reallocating insert @ 0x00BEF830 (manifest "CRandomAppearanceMorph_Reallocate").
// Label correction: the manifest calls this "CRandomAppearanceMorph::PushMorph" (a propagated name; the 0x00BExxxx module
// is unrelated to CRandomAppearanceMorph @ 0x0086xxxx). It is the out-of-line push_back of the entry vector of the table
// that AddEntry fills (vector at +0x00 of the table, the dirty flag at +0x0d), so the record types reuse AddEntry's model.
// Shape: the retail VC6-era push_back -- `if (last != end) { new (last) T(value); ++last; } else InsertGrow(last, value,
// SScalarTag(), 1, true);` (`je` to the grow arm first, the null-checked placement copy, `add dword ptr [esi+4],1Ch`); the
// record's implicit copy runs the key's two words and the value's user copy constructor word by word through edx before
// the vector copy; the empty tag temporary is homed in the dead `value` argument slot (`lea ecx,[esp+10h]`).
// Leaf: push_back
#include <new>

struct SScalarTag { SScalarTag() { } };

struct CRetailVector_00befbc0 {                         // 12 bytes: first, last, end of storage
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CRetailVector_00befbc0(const CRetailVector_00befbc0& other);   // 0x005BB346
};
struct SKey_00befbc0 {                                  // 8 bytes, plain
    long m_a;
    long m_b;
};
struct SValue_00befbc0 {                                // 20 bytes: two words and the vector
    long m_c;
    long m_d;
    CRetailVector_00befbc0 m_items;
    SValue_00befbc0(const SValue_00befbc0& other) : m_c(other.m_c), m_d(other.m_d), m_items(other.m_items) {}
};
struct SEntry_00befbc0 {                                // 28 bytes: the pair pushed into the table
    SKey_00befbc0   first;
    SValue_00befbc0 second;
};

struct CEntryVector_00bef9c0 {
    SEntry_00befbc0* first;                             // +0x00
    SEntry_00befbc0* last;                              // +0x04
    SEntry_00befbc0* end;                               // +0x08
    void InsertGrow(SEntry_00befbc0* where, const SEntry_00befbc0& value, const SScalarTag& tag,
                    unsigned long count, bool noTail);  // 0x00BEF830
    void push_back(const SEntry_00befbc0& value);
};

void CEntryVector_00bef9c0::push_back(const SEntry_00befbc0& value)
{
    if (last != end) {
        new (last) SEntry_00befbc0(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}