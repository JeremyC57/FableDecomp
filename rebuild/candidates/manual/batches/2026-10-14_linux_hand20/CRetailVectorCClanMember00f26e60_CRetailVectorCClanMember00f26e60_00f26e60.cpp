// CRetailVector_CClanMember_00f26e60::CRetailVector_CClanMember_00f26e60(const CRetailVector_CClanMember_00f26e60&) -- std::vector<CClanMember> copy constructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F26E60, length 93 bytes.
// Callees: the vector base constructor @ 0x00F26EC0 (zero the three pointers, allocate n elements, set
// first/last/end), the element placement copy @ 0x00F26F00 (`__fastcall`, destination in ecx, source in
// edx: the element's copy constructor).
// The manifest label (CCombatWheel::ResetRings) is propagated; this body is a 56-byte-element vector copy
// (callers: the clan-member _Cons_val @ 0x00F26DB0, 0x00F35780; size() is the signed /56 reciprocal multiply). Shape: base-constructed with other.size() and a copied allocator (an empty temporary
// homed in the dead `other` argument slot), then uninitialized_copy into `first` with other's bounds in
// callee-saved registers, and the running destination stored as `last`. Named by what it does.
struct CRetailAllocator { CRetailAllocator() { } };   // empty; the user constructor keeps the temporary store-free

struct CClanMember {                                    // CTCHeroOnlineScoreboard::CClanMember {vptr 0x012FB498, ...} 56 bytes
    unsigned char _body[0x38];
};

void __fastcall ConstructCopy_CClanMember(CClanMember* dest, const CClanMember& src);   // 0x00F26F00

struct CRetailVectorBase_CClanMember {
    CClanMember* first;
    CClanMember* last;
    CClanMember* end_of_storage;
    CRetailVectorBase_CClanMember(unsigned long n, const CRetailAllocator& allocator);   // 0x00F26EC0
};

struct CRetailVector_CClanMember_00f26e60 : CRetailVectorBase_CClanMember {
    CRetailVector_CClanMember_00f26e60(const CRetailVector_CClanMember_00f26e60& other);
    unsigned long size() const { return (unsigned long)(last - first); }
    CRetailAllocator get_allocator() const { return CRetailAllocator(); }
};

// SGI uninitialized_copy for a non-trivial element type: placement-copy each element, return the end.
static inline CClanMember* UninitializedCopy_CClanMember(const CClanMember* first, const CClanMember* last, CClanMember* result)
{
    for (; first != last; ++first, ++result)
        ConstructCopy_CClanMember(result, *first);
    return result;
}

CRetailVector_CClanMember_00f26e60::CRetailVector_CClanMember_00f26e60(const CRetailVector_CClanMember_00f26e60& other)
    : CRetailVectorBase_CClanMember(other.size(), other.get_allocator())
{
    last = UninitializedCopy_CClanMember(other.first, other.last, first);
}
