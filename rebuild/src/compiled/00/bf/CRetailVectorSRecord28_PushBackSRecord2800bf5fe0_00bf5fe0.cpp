// CRetailVector_SRecord28::PushBack_SRecord28_00bf5fe0(const SRecord28& value) -- retail vector push_back of a 28-byte record: placement-copy at `last` when there is room, else InsertGrow @ 0x00BF5EE0
// Leaf: PushBack_SRecord28_00bf5fe0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF5FE0, length 88 bytes.
// Callees: the reallocating insert @ 0x00BF5EE0 (one element, `noTail` set) when the vector is full.
// Shape: `this` stays in ecx (no frame); the element has an explicit memberwise copy constructor, so the placement copy is 7 direct
// dword moves through esi (a POD element of this width would be a `rep movsd` block copy).
// The full branch passes a const reference to an empty iterator-category tag whose 1-byte temporary
// VC homes in this function's dead argument slot (`lea edx,[esp+0Ch]` after the two `push 1`).
// The manifest label "CScriptModuleTransitionBetweenPoints::OnInitLeaf" is propagated from an unrelated body (the bytes are
// a vector {first, last, end} push_back); named by element size and address.
#include <new>

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary

struct SRecord28 {                     // 28-byte element: 7 dwords, explicit memberwise copy constructor
    long m_a, m_b, m_c, m_d, m_e, m_f, m_g;
    SRecord28() { }
    SRecord28(const SRecord28& other) : m_a(other.m_a), m_b(other.m_b), m_c(other.m_c), m_d(other.m_d), m_e(other.m_e), m_f(other.m_f), m_g(other.m_g) { }
};

struct CRetailVector_SRecord28 {
    SRecord28* first;
    SRecord28* last;
    SRecord28* end;
    void InsertGrow(SRecord28* where, const SRecord28& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00BF5EE0
    void PushBack_SRecord28_00bf5fe0(const SRecord28& value);
};

void CRetailVector_SRecord28::PushBack_SRecord28_00bf5fe0(const SRecord28& value)
{
    if (last != end) {
        new (last) SRecord28(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}