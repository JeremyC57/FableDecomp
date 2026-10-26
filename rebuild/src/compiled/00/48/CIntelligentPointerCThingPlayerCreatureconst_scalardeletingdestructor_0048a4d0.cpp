// CIntelligentPointer::`vector_deleting_destructor'
// Canonical MSVC vector deleting destructor.

struct CIntelligentPointer
{
    void scalar_destructor();   // scalar dtor at 0xa01c10 (external, non-inlined)
};


void* __fastcall CIntelligentPointer_scalar_deleting_destructor(
        CIntelligentPointer* pointer, int /*edx*/, unsigned int flags)
{
    pointer->scalar_destructor();
    if (flags & 1)
        operator delete(pointer);
    return pointer;
}