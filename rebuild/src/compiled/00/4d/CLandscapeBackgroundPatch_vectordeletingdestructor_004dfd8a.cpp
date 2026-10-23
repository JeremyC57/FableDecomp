#pragma optimize("s", on)
extern void __fastcall AutoTinyScalarDestructor_004db5bb(void* self);
extern void __cdecl AutoTinyScalarDelete(void* object);
void* __fastcall CLandscapeBackgroundPatch_ScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyScalarDestructor_004db5bb(self);
    if (flags & 1)
        AutoTinyScalarDelete(self);
    return self;
}