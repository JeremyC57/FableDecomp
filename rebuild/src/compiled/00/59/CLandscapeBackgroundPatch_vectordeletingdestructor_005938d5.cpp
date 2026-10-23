#pragma optimize("s", on)
extern void __fastcall AutoTinyScalarDestructor_005930f3(void* self);
extern void __cdecl AutoTinyScalarDelete(void* object);
void* __fastcall CLandscapeBackgroundPatch_ScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyScalarDestructor_005930f3(self);
    if (flags & 1)
        AutoTinyScalarDelete(self);
    return self;
}