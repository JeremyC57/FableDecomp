// CRetailVector_CScriptThing_Retail_00dfde30::CRetailVector_CScriptThing_Retail_00dfde30(const CRetailVector_CScriptThing_Retail_00dfde30&) -- std::vector<CScriptThing_Retail> copy constructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DFDE30, length 93 bytes.
// Callees: the vector base constructor @ 0x00DFDE90 (zero the three pointers, allocate n elements, set
// first/last/end), the element placement copy @ 0x008AC120 (`__fastcall`, destination in ecx, source in
// edx: store the vtable, copy the counted pointer and bump its count).
// The manifest label (CCombatWheel::ResetRings) is propagated; this body is a 12-byte-element vector copy
// (caller: CQ_TraderConflictGoodScript::AttackPeople @ 0x00DFD600; size() is the signed /12 reciprocal multiply). Shape: base-constructed with other.size() and a copied allocator (an empty temporary
// homed in the dead `other` argument slot), then uninitialized_copy into `first` with other's bounds in
// callee-saved registers, and the running destination stored as `last`. Named by what it does.
struct CRetailAllocator { CRetailAllocator() { } };   // empty; the user constructor keeps the temporary store-free

struct CScriptThing_Retail {                                    // CScriptThing {vptr 0x01238C8C, object, count block}; engine/CScriptThing.h exists but only the 12-byte size matters here
    unsigned char _body[0xc];
};

void __fastcall ConstructCopy_CScriptThing_Retail(CScriptThing_Retail* dest, const CScriptThing_Retail& src);   // 0x008AC120

struct CRetailVectorBase_CScriptThing_Retail {
    CScriptThing_Retail* first;
    CScriptThing_Retail* last;
    CScriptThing_Retail* end_of_storage;
    CRetailVectorBase_CScriptThing_Retail(unsigned long n, const CRetailAllocator& allocator);   // 0x00DFDE90
};

struct CRetailVector_CScriptThing_Retail_00dfde30 : CRetailVectorBase_CScriptThing_Retail {
    CRetailVector_CScriptThing_Retail_00dfde30(const CRetailVector_CScriptThing_Retail_00dfde30& other);
    unsigned long size() const { return (unsigned long)(last - first); }
    CRetailAllocator get_allocator() const { return CRetailAllocator(); }
};

// SGI uninitialized_copy for a non-trivial element type: placement-copy each element, return the end.
static inline CScriptThing_Retail* UninitializedCopy_CScriptThing_Retail(const CScriptThing_Retail* first, const CScriptThing_Retail* last, CScriptThing_Retail* result)
{
    for (; first != last; ++first, ++result)
        ConstructCopy_CScriptThing_Retail(result, *first);
    return result;
}

CRetailVector_CScriptThing_Retail_00dfde30::CRetailVector_CScriptThing_Retail_00dfde30(const CRetailVector_CScriptThing_Retail_00dfde30& other)
    : CRetailVectorBase_CScriptThing_Retail(other.size(), other.get_allocator())
{
    last = UninitializedCopy_CScriptThing_Retail(other.first, other.last, first);
}