// CMemoryAllocator::`vector_deleting_destructor'
// push esi; mov esi,ecx; call dtor(0x9f2310); test [esp+8],1; je +;
// mov ecx,esi; call free(0x9f1f90); +: mov eax,esi; pop esi; ret 4
// Both callees are __fastcall (this in ecx). First unconditional, second flag-gated.

struct CMemoryAllocator {
    int dummy;
    void Destroy();      // 0x9f2310  (scalar destructor body, this in ecx)
    void FreeThis();     // 0x9f1f90  (this in ecx)
};

void* __fastcall CMemoryAllocator_scalar_deleting_destructor(
        CMemoryAllocator* allocator, int /*edx*/, unsigned int flags)
{
    allocator->Destroy();
    if (flags & 1)
        allocator->FreeThis();
    return allocator;
}