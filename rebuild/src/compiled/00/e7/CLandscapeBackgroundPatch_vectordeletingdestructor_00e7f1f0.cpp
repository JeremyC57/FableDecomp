struct CLandscapeBackgroundPatch {
    void* vftable;
};

void __fastcall CLandscapeBackgroundPatch_dtor_00e7f210(CLandscapeBackgroundPatch* self);
void operator delete(void*);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_unused, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor_00e7f210(self);
    if (flags & 1)
        operator delete(self);
    return self;
}