// CInputProcessTargetLockCycleTargets `vector_deleting_destructor'
// retail 0x0048b4a0 __fastcall(this, uint flags) -> void*

// operator delete (retail 0xbfe9bc)

class CInputProcessTargetLockCycleTargets
{ public:
    void destroy();  // real destructor body @ 0xa0d3d0
};

// scalar/vector deleting destructor shape
void* __fastcall CInputProcessTargetLockCycleTargets_scalar_deleting_destructor(
        CInputProcessTargetLockCycleTargets* process, int /*edx*/, unsigned int flags)
{
    process->destroy();
    if (flags & 1)
        operator delete(process);
    return process;
}