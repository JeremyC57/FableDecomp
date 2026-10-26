// CInputProcessInventoryBase::`vector_deleting_destructor' 0x0048b4e0
// Standard MSVC vector-deleting destructor thunk.

struct CInputProcessInventoryBase
{
    void ScalarDtor();           // 0x00a0d3d0  (the ~destructor body)
};

// operator delete (0x00bfe9bc)

void* __fastcall CInputProcessInventoryBase_scalar_deleting_destructor(
    CInputProcessInventoryBase* process, int /*edx*/, unsigned int flags)
{
    process->ScalarDtor();
    if (flags & 1)
        operator delete(process);
    return process;
}