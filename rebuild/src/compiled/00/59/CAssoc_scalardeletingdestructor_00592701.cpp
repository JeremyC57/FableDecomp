#pragma optimize("s", on)
extern void __fastcall AutoTinyScalarDestructor_00591f62(void* self);
extern void __cdecl AutoTinyScalarDelete(void* object);
void* __fastcall CAssoc_ScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyScalarDestructor_00591f62(self);
    if (flags & 1)
        AutoTinyScalarDelete(self);
    return self;
}