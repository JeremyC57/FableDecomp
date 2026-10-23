#pragma optimize("s", on)
extern void __fastcall AutoTinyScalarDestructor_00414ea0(void* self);
extern void __cdecl AutoTinyScalarDelete(void* object);
void* __fastcall CLandscapeBackgroundPatch_ScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyScalarDestructor_00414ea0(self);
    if (flags & 1)
        AutoTinyScalarDelete(self);
    return self;
}