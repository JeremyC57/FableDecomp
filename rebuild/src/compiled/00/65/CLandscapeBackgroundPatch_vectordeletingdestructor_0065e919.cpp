#pragma optimize("s", on)
// Leaf: `vector_deleting_destructor'
extern void __fastcall AutoTinyScalarDestructor_0065d574(void* self);
void* __fastcall CLandscapeBackgroundPatch_ScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyScalarDestructor_0065d574(self);
    if (flags & 1)
        operator delete(self);
    return self;
}