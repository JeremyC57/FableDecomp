// CRetailVector_CCountedPointer_0062edd0::CRetailVector_CCountedPointer_0062edd0(const CRetailVector_CCountedPointer_0062edd0&) -- std::vector<CCountedPointer> copy constructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062EDD0, length 80 bytes.
// Callees: the vector base constructor @ 0x0062EE20 (zero the three pointers, allocate n elements, set
// first/last/end), the element placement copy @ 0x0062EE60 (`__fastcall`, destination in ecx, source in
// edx: copy both dwords and bump the count).
// The manifest label (CCombatWheel::ResetRings) is propagated; this body is a 8-byte-element vector copy
// (caller: CCreatureActionBase::CCreatureActionBase @ 0x0062EBC0; the element copy is the CCountedPointer shape). Shape: base-constructed with other.size() and a copied allocator (an empty temporary
// homed in the dead `other` argument slot), then uninitialized_copy into `first` with other's bounds in
// callee-saved registers, and the running destination stored as `last`. Named by what it does.
struct CRetailAllocator { CRetailAllocator() { } };   // empty; the user constructor keeps the temporary store-free

struct CCountedPointer {                                    // CCountedPointer<T> {object, count block}
    unsigned char _body[0x8];
};

void __fastcall ConstructCopy_CCountedPointer(CCountedPointer* dest, const CCountedPointer& src);   // 0x0062EE60

struct CRetailVectorBase_CCountedPointer {
    CCountedPointer* first;
    CCountedPointer* last;
    CCountedPointer* end_of_storage;
    CRetailVectorBase_CCountedPointer(unsigned long n, const CRetailAllocator& allocator);   // 0x0062EE20
};

struct CRetailVector_CCountedPointer_0062edd0 : CRetailVectorBase_CCountedPointer {
    CRetailVector_CCountedPointer_0062edd0(const CRetailVector_CCountedPointer_0062edd0& other);
    unsigned long size() const { return (unsigned long)(last - first); }
    CRetailAllocator get_allocator() const { return CRetailAllocator(); }
};

// SGI uninitialized_copy for a non-trivial element type: placement-copy each element, return the end.
static inline CCountedPointer* UninitializedCopy_CCountedPointer(const CCountedPointer* first, const CCountedPointer* last, CCountedPointer* result)
{
    for (; first != last; ++first, ++result)
        ConstructCopy_CCountedPointer(result, *first);
    return result;
}

CRetailVector_CCountedPointer_0062edd0::CRetailVector_CCountedPointer_0062edd0(const CRetailVector_CCountedPointer_0062edd0& other)
    : CRetailVectorBase_CCountedPointer(other.size(), other.get_allocator())
{
    last = UninitializedCopy_CCountedPointer(other.first, other.last, first);
}