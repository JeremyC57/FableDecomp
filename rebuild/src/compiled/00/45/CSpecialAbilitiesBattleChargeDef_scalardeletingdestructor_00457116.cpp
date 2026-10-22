#pragma optimize("s",on)
void operator delete(void*);
void __fastcall CLandscapeBackgroundPatch_dtor(void* self);

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(void* self, int edx, unsigned int flags)
{
    CLandscapeBackgroundPatch_dtor(self);
    if (flags & 1)
        operator delete(self);
    return self;
}