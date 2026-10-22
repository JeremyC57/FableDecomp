// CIntelligentPointer::`vector_deleting_destructor'
// Canonical MSVC vector deleting destructor.

struct CIntelligentPointer
{
    void scalar_destructor();   // scalar dtor at 0xa01c10 (external, non-inlined)
};

extern void __cdecl fable_operator_delete(void*);  // operator delete at 0xbfe9bc

void* __fastcall CIntelligentPointer_scalar_deleting_destructor(
        CIntelligentPointer* pointer, int /*edx*/, unsigned int flags)
{
    pointer->scalar_destructor();
    if (flags & 1)
        fable_operator_delete(pointer);
    return pointer;
}