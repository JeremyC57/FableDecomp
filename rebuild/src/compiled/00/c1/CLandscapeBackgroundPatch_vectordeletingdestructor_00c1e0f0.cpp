class CLandscapeBackgroundPatch { public:
// Leaf: `vector_deleting_destructor'
    void* vftable;
};

void __fastcall CLandscapeBackgroundPatch_dtor_00c1e080(CLandscapeBackgroundPatch* self);
void operator delete(void*);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor_00c1e080(self);
    if (flags & 1)
        operator delete(self);
    return self;
}