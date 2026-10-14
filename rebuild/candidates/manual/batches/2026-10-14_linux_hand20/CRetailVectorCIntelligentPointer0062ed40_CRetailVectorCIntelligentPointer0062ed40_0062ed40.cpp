// CRetailVector_CIntelligentPointer_0062ed40::CRetailVector_CIntelligentPointer_0062ed40(const CRetailVector_CIntelligentPointer_0062ed40&) -- std::vector<CIntelligentPointer> copy constructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062ED40, length 80 bytes.
// Callees: the vector base constructor @ 0x0062ED90 (zero the three pointers, allocate n elements, set
// first/last/end), the element placement copy @ 0x004AA030 (`__fastcall`, destination in ecx, source in
// edx: base-construct, store the vtable, SetPItem(src.GetPItem())).
// The manifest label (CCombatWheel::ResetRings) is propagated; this body is a 8-byte-element vector copy
// (callers: CCreatureActionBase::CCreatureActionBase @ 0x0062EBC0, Region_RemoveDeadThings @ 0x006C3E80). Shape: base-constructed with other.size() and a copied allocator (an empty temporary
// homed in the dead `other` argument slot), then uninitialized_copy into `first` with other's bounds in
// callee-saved registers, and the running destination stored as `last`. Named by what it does.
struct CRetailAllocator { CRetailAllocator() { } };   // empty; the user constructor keeps the temporary store-free

struct CIntelligentPointer {                                    // CIntelligentPointer<CThing const> {vptr 0x01238C6C, pItem}
    unsigned char _body[0x8];
};

void __fastcall ConstructCopy_CIntelligentPointer(CIntelligentPointer* dest, const CIntelligentPointer& src);   // 0x004AA030

struct CRetailVectorBase_CIntelligentPointer {
    CIntelligentPointer* first;
    CIntelligentPointer* last;
    CIntelligentPointer* end_of_storage;
    CRetailVectorBase_CIntelligentPointer(unsigned long n, const CRetailAllocator& allocator);   // 0x0062ED90
};

struct CRetailVector_CIntelligentPointer_0062ed40 : CRetailVectorBase_CIntelligentPointer {
    CRetailVector_CIntelligentPointer_0062ed40(const CRetailVector_CIntelligentPointer_0062ed40& other);
    unsigned long size() const { return (unsigned long)(last - first); }
    CRetailAllocator get_allocator() const { return CRetailAllocator(); }
};

// SGI uninitialized_copy for a non-trivial element type: placement-copy each element, return the end.
static inline CIntelligentPointer* UninitializedCopy_CIntelligentPointer(const CIntelligentPointer* first, const CIntelligentPointer* last, CIntelligentPointer* result)
{
    for (; first != last; ++first, ++result)
        ConstructCopy_CIntelligentPointer(result, *first);
    return result;
}

CRetailVector_CIntelligentPointer_0062ed40::CRetailVector_CIntelligentPointer_0062ed40(const CRetailVector_CIntelligentPointer_0062ed40& other)
    : CRetailVectorBase_CIntelligentPointer(other.size(), other.get_allocator())
{
    last = UninitializedCopy_CIntelligentPointer(other.first, other.last, first);
}
