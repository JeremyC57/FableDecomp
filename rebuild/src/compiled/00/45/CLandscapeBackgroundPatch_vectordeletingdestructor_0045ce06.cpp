#pragma optimize("s", on)
extern void __fastcall AutoTinyScalarDestructor_0045ce22(void* self);
extern void __cdecl AutoTinyScalarDelete(void* object);
void* __fastcall CLandscapeBackgroundPatch_ScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyScalarDestructor_0045ce22(self);
    if (flags & 1)
        AutoTinyScalarDelete(self);
    return self;
}