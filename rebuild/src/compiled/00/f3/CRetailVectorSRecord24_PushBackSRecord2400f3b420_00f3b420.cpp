// CRetailVector_SRecord24::PushBack_SRecord24_00f3b420(const SRecord24& value) -- retail vector push_back of a 24-byte record: placement-copy at `last` when there is room, else InsertGrow @ 0x00F3B330
// Leaf: PushBack_SRecord24_00f3b420
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F3B420, length 82 bytes.
// Callees: the reallocating insert @ 0x00F3B330 (one element, `noTail` set) when the vector is full.
// Shape: `this` stays in ecx (no frame); the element has an explicit memberwise copy constructor, so the placement copy is 6 direct
// dword moves through esi (a POD element of this width would be a `rep movsd` block copy).
// The full branch passes a const reference to an empty iterator-category tag whose 1-byte temporary
// VC homes in this function's dead argument slot (`lea edx,[esp+0Ch]` after the two `push 1`).
// The manifest label "CScriptModuleFullTransition::OnInitLeaf" is propagated from an unrelated body (the bytes are
// a vector {first, last, end} push_back); named by element size and address.
#include <new>

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary

struct SRecord24 {                     // 24-byte element: 6 dwords, explicit memberwise copy constructor
    long m_a, m_b, m_c, m_d, m_e, m_f;
    SRecord24() { }
    SRecord24(const SRecord24& other) : m_a(other.m_a), m_b(other.m_b), m_c(other.m_c), m_d(other.m_d), m_e(other.m_e), m_f(other.m_f) { }
};

struct CRetailVector_SRecord24 {
    SRecord24* first;
    SRecord24* last;
    SRecord24* end;
    void InsertGrow_00f3b330(SRecord24* where, const SRecord24& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00F3B330
    void PushBack_SRecord24_00f3b420(const SRecord24& value);
};

void CRetailVector_SRecord24::PushBack_SRecord24_00f3b420(const SRecord24& value)
{
    if (last != end) {
        new (last) SRecord24(value);
        ++last;
    }
    else
        InsertGrow_00f3b330(last, value, SScalarTag(), 1, true);
}