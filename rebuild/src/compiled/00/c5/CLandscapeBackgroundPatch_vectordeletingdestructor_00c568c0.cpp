class CLandscapeBackgroundPatch { public:
// Leaf: `vector_deleting_destructor'
    void* vftable;
};

void __fastcall CLandscapeBackgroundPatch_dtor_00c568e0(CLandscapeBackgroundPatch* self);
extern void free_engine(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor_00c568e0(self);
    if (flags & 1)
        free_engine(self);
    return self;
}