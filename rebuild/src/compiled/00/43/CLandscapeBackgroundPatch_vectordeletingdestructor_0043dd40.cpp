class CLandscapeBackgroundPatch { public: void* vftable; };
// Leaf: `vector_deleting_destructor'
extern "C" void __fastcall CLandscapeBackgroundPatch_scalar_dtor_0099b510(CLandscapeBackgroundPatch* self);
extern "C" void __cdecl CLandscapeBackgroundPatch_op_delete(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_dummy, unsigned int flags)
{
    (void)edx_dummy;
    CLandscapeBackgroundPatch_scalar_dtor_0099b510(self);
    if (flags & 1)
        CLandscapeBackgroundPatch_op_delete(self);
    return self;
}