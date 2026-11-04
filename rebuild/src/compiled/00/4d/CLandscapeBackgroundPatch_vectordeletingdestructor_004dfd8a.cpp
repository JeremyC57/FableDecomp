#pragma optimize("s", on)
// Leaf: ?CLandscapeBackgroundPatch_ScalarDeletingDestructor@@YIPAXPAXHI@Z
extern void __fastcall AutoTinyScalarDestructor_004db5bb(void* self);
void* __fastcall CLandscapeBackgroundPatch_ScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyScalarDestructor_004db5bb(self);
    if (flags & 1)
        operator delete(self);
    return self;
}