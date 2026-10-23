struct CLandscapeBackgroundPatch {
    void* vftable;
};

void __fastcall CLandscapeBackgroundPatch_dtor_00c55540(CLandscapeBackgroundPatch* self);
extern void free_engine(void* p);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor_00c55540(self);
    if (flags & 1)
        free_engine(self);
    return self;
}