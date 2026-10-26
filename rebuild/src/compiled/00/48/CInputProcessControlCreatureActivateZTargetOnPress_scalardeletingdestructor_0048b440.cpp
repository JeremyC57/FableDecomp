// CInputProcessControlCreatureActivateZTargetOnPress::`vector_deleting_destructor'
// Standard MSVC vector deleting destructor shape.

class CInputProcessControlCreatureActivateZTargetOnPress
{ public:
    void dtor();  // scalar destructor at 0xa0d3d0
};


void* __fastcall CInputProcessControlCreatureActivateZTargetOnPress_scalar_deleting_destructor(
    CInputProcessControlCreatureActivateZTargetOnPress* process, int /*edx*/, unsigned int flags)
{
    process->dtor();
    if (flags & 1)
        operator delete(process);
    return process;
}