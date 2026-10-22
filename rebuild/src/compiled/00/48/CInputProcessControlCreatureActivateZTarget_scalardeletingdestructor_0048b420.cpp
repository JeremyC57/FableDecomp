// CInputProcessControlCreatureActivateZTarget::`vector_deleting_destructor'
// __fastcall (this in ecx), uint flags on stack, returns this.

struct CInputProcessControlCreatureActivateZTarget {
    // scalar destructor lives at 0xa0d3d0 in retail; model as an extern.
};

extern "C" void __fastcall LandscapeBackgroundPatch_dtor(CInputProcessControlCreatureActivateZTarget* process);
extern "C" void operator_delete_impl(void* p);

void* __fastcall CInputProcessControlCreatureActivateZTarget_scalar_deleting_destructor(
        CInputProcessControlCreatureActivateZTarget* process, int /*edx*/, unsigned int flags)
{
    LandscapeBackgroundPatch_dtor(process);
    if (flags & 1)
        operator_delete_impl(process);
    return process;
}