struct CLandscapeBackgroundPatch {
    void* vftable;
};

void __fastcall CLandscapeBackgroundPatch_dtor_00c31f00(CLandscapeBackgroundPatch* patch);
void operator delete(void*);

void* __fastcall CLandscapeBackgroundPatch_scalar_deleting_destructor(CLandscapeBackgroundPatch* patch, int edx_unused, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor_00c31f00(patch);
    if (flags & 1)
        operator delete(patch);
    return patch;
}