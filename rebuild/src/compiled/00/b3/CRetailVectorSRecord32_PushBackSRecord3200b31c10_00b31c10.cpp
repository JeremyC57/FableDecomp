// CRetailVector_SRecord32::PushBack_SRecord32_00b31c10(const SRecord32& value) -- retail vector push_back of a 32-byte record: placement-copy at `last` when there is room, else InsertGrow @ 0x00B31B10
// Leaf: PushBack_SRecord32_00b31c10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B31C10, length 94 bytes.
// Callees: the reallocating insert @ 0x00B31B10 (one element, `noTail` set) when the vector is full.
// Shape: `this` stays in ecx (no frame); the element has an explicit memberwise copy constructor, so the placement copy is 8 direct
// dword moves through esi (a POD element of this width would be a `rep movsd` block copy).
// The full branch passes a const reference to an empty iterator-category tag whose 1-byte temporary
// VC homes in this function's dead argument slot (`lea edx,[esp+0Ch]` after the two `push 1`).
// The manifest label "CScriptModuleTransitionBetweenPoints::OnInitLeaf" is propagated from an unrelated body (the bytes are
// a vector {first, last, end} push_back); named by element size and address.
#include <new>

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary

struct SRecord32 {                     // 32-byte element: 8 dwords, explicit memberwise copy constructor
    long m_a, m_b, m_c, m_d, m_e, m_f, m_g, m_h;
    SRecord32() { }
    SRecord32(const SRecord32& other) : m_a(other.m_a), m_b(other.m_b), m_c(other.m_c), m_d(other.m_d), m_e(other.m_e), m_f(other.m_f), m_g(other.m_g), m_h(other.m_h) { }
};

struct CRetailVector_SRecord32 {
    SRecord32* first;
    SRecord32* last;
    SRecord32* end;
    void InsertGrow(SRecord32* where, const SRecord32& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00B31B10
    void PushBack_SRecord32_00b31c10(const SRecord32& value);
};

void CRetailVector_SRecord32::PushBack_SRecord32_00b31c10(const SRecord32& value)
{
    if (last != end) {
        new (last) SRecord32(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}