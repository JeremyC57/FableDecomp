#pragma optimize("s", on)
// Leaf: `vector_deleting_destructor'
extern void __fastcall AutoTinyScalarDestructor_0065d5a0(void* self);
extern void __cdecl AutoTinyScalarDelete(void* object);
void* __fastcall CLandscapeBackgroundPatch_ScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyScalarDestructor_0065d5a0(self);
    if (flags & 1)
        AutoTinyScalarDelete(self);
    return self;
}