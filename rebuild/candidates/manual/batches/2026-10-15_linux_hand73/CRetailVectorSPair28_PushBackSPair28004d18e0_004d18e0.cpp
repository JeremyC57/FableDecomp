// CRetailVector_SPair28::PushBack_SPair28_004d18e0(const SPair28& value) -- retail vector push_back of a 28-byte record: placement-copy at `last` when there is room, else InsertGrow @ 0x004D1360
// Leaf: PushBack_SPair28_004d18e0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D18E0, length 92 bytes.
// Callees: the reallocating insert @ 0x004D1360 (one element, `noTail` set) when the vector is full.
// Shape: `this` stays in ecx (no frame); the element is a pair-style record {long key; POD value} whose copy constructor copies the key,
// then the 24-byte POD value as a whole: VC re-bases both pointers (`add edx,4 ; add eax,4`) and
// moves the value dword by dword (a memberwise value copy constructor folds the offsets instead).
// The full branch passes a const reference to an empty iterator-category tag whose 1-byte temporary
// VC homes in this function's dead argument slot (`lea edx,[esp+0Ch]` after the two `push 1`).
// The manifest label "CScriptModuleTransitionBetweenPoints::OnInitLeaf" is propagated from an unrelated body (the bytes are
// a vector {first, last, end} push_back); named by element size and address.
#include <new>

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary

struct SValue24 {                     // plain POD record: copied as a whole through re-based pointers
    long m_a, m_b, m_c, m_d, m_e, m_f;
};

struct SPair28 {                      // 28-byte element: pair<long, SValue24> with a pair-style copy constructor
    long m_key;
    SValue24 m_value;
    SPair28() { }
    SPair28(const SPair28& other) : m_key(other.m_key), m_value(other.m_value) { }
};

struct CRetailVector_SPair28 {
    SPair28* first;
    SPair28* last;
    SPair28* end;
    void InsertGrow(SPair28* where, const SPair28& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x004D1360
    void PushBack_SPair28_004d18e0(const SPair28& value);
};

void CRetailVector_SPair28::PushBack_SPair28_004d18e0(const SPair28& value)
{
    if (last != end) {
        new (last) SPair28(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}
