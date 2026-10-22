// CInputProcessControlCreatureActivateZTargetOnPress::`vector_deleting_destructor'
// Standard MSVC vector deleting destructor shape.

struct CInputProcessControlCreatureActivateZTargetOnPress
{
    void dtor();  // scalar destructor at 0xa0d3d0
};

extern void __cdecl fable_operator_delete(void* p);  // 0xbfe9bc

void* __fastcall CInputProcessControlCreatureActivateZTargetOnPress_scalar_deleting_destructor(
    CInputProcessControlCreatureActivateZTargetOnPress* process, int /*edx*/, unsigned int flags)
{
    process->dtor();
    if (flags & 1)
        fable_operator_delete(process);
    return process;
}