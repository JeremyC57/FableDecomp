class CLandscapeBackgroundPatch { public: void* vftable; };
// Leaf: `vector_deleting_destructor'
extern "C" void __fastcall CLandscapeBackgroundPatch_scalar_dtor_0099b510(CLandscapeBackgroundPatch* self);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_dummy, unsigned int flags)
{
    (void)edx_dummy;
    CLandscapeBackgroundPatch_scalar_dtor_0099b510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}