class CLandscapeBackgroundPatch;
// Leaf: `vector_deleting_destructor'
extern "C" void __fastcall CLandscapeBackgroundPatch_dtor(CLandscapeBackgroundPatch* self);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx_pad*/, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1) {
        operator delete((void*)self);
    }
    return (void*)self;
}