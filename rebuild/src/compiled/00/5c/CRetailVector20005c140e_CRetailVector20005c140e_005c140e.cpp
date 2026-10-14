#pragma optimize("y", off)
#pragma optimize("s", on)
// CRetailVector20_005c140e::CRetailVector20_005c140e(const CRetailVector20_005c140e& other) -- the VC6 vector copy constructor for 20-byte elements: allocate for the source's count (its byte span divided by 20), then uninitialised-copy the range and set the end
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C140E, length 60 bytes.
// Callees: the initialiser @ 0x005C0932 (on `this`: the count and the allocator tag by address), the range copy @ 0x005C10CC (`__fastcall`: the source first / last, this
// vector's first and the tag; returns the new end). Shape: a frame pointer and the size-optimised module (`push 14h ; pop ecx ; cdq ; idiv ecx`), the tag
// temporary homed at `[ebp+0Bh]`; `this` is returned, `ret 4`. The chunk label "CExpressionSet::Add" is propagated; named by effect.
struct SScalarTag { SScalarTag() { } };

struct SElement20_005c140e { unsigned char _body[20]; };
SElement20_005c140e* __fastcall UninitCopy_005c10cc(SElement20_005c140e* first, SElement20_005c140e* last, SElement20_005c140e* dest, const SScalarTag& tag);   // 0x005C10CC

struct CRetailVector20_005c140e {
    SElement20_005c140e* m_pFirst;
    SElement20_005c140e* m_pLast;
    SElement20_005c140e* m_pEnd;
    void Initialize(long count, const SScalarTag& tag);   // 0x005C0932
    CRetailVector20_005c140e(const CRetailVector20_005c140e& other);
};

CRetailVector20_005c140e::CRetailVector20_005c140e(const CRetailVector20_005c140e& other)
{
    Initialize((long)((char*)other.m_pLast - (char*)other.m_pFirst) / 20, SScalarTag());
    m_pLast = UninitCopy_005c10cc(other.m_pFirst, other.m_pLast, m_pFirst, SScalarTag());
}