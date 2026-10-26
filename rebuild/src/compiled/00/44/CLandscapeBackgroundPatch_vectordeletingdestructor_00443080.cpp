class CLandscapeBackgroundPatch { public:
// Leaf: `vector_deleting_destructor'
    void* vftable;
    void* CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags);
};
extern "C" void __fastcall CLandscapeBackgroundPatch_scalar_dtor(CLandscapeBackgroundPatch* self);

void* CLandscapeBackgroundPatch::CLandscapeBackgroundPatch_vector_deleting_destructor(unsigned int flags)
{
    CLandscapeBackgroundPatch_scalar_dtor(this);
    if (flags & 1)
        operator delete((void*)this);
    return (void*)this;
}