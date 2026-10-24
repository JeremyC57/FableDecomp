struct CLandscapeBackgroundPatch {
// Leaf: `vector_deleting_destructor'
    void* vftable;
};

void __fastcall CLandscapeBackgroundPatch_dtor_006b1550(CLandscapeBackgroundPatch* self);
void operator delete(void*);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor_006b1550(self);
    if (flags & 1)
        operator delete(self);
    return self;
}