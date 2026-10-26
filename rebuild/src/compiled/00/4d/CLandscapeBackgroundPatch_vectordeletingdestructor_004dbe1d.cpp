#pragma optimize("s", on)
// Leaf: `vector_deleting_destructor'
extern void __fastcall AutoTinyScalarDestructor_004d78ed(void* self);
void* __fastcall CLandscapeBackgroundPatch_ScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyScalarDestructor_004d78ed(self);
    if (flags & 1)
        operator delete(self);
    return self;
}