#pragma optimize("y", off)
#pragma optimize("s", on)
// CRetailVector20_005bbdf1::CRetailVector20_005bbdf1(const CRetailVector20_005bbdf1& other) -- the VC6 vector copy constructor for 20-byte elements: allocate for the source's count (its byte span divided by 20), then uninitialised-copy the range and set the end
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BBDF1, length 60 bytes.
// Callees: the initialiser @ 0x005BA5AD (on `this`: the count and the allocator tag by address), the range copy @ 0x005BBC93 (`__fastcall`: the source first / last, this
// vector's first and the tag; returns the new end). Shape: a frame pointer and the size-optimised module (`push 14h ; pop ecx ; cdq ; idiv ecx`), the tag
// temporary homed at `[ebp+0Bh]`; `this` is returned, `ret 4`. The chunk label "CExpressionSet::Add" is propagated; named by effect.
struct SScalarTag { SScalarTag() { } };

struct SElement20_005bbdf1 { unsigned char _body[20]; };
SElement20_005bbdf1* __fastcall UninitCopy_005bbc93(SElement20_005bbdf1* first, SElement20_005bbdf1* last, SElement20_005bbdf1* dest, const SScalarTag& tag);   // 0x005BBC93

struct CRetailVector20_005bbdf1 {
    SElement20_005bbdf1* m_pFirst;
    SElement20_005bbdf1* m_pLast;
    SElement20_005bbdf1* m_pEnd;
    void Initialize(long count, const SScalarTag& tag);   // 0x005BA5AD
    CRetailVector20_005bbdf1(const CRetailVector20_005bbdf1& other);
};

CRetailVector20_005bbdf1::CRetailVector20_005bbdf1(const CRetailVector20_005bbdf1& other)
{
    Initialize((long)((char*)other.m_pLast - (char*)other.m_pFirst) / 20, SScalarTag());
    m_pLast = UninitCopy_005bbc93(other.m_pFirst, other.m_pLast, m_pFirst, SScalarTag());
}