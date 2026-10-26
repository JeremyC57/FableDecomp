#pragma optimize("s", on)
// Leaf: `vector_deleting_destructor'
extern void __fastcall AutoTinyScalarDestructor_005c2739(void* self);
void* __fastcall CLandscapeBackgroundPatch_ScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyScalarDestructor_005c2739(self);
    if (flags & 1)
        operator delete(self);
    return self;
}