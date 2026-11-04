#pragma optimize("s", on)
// Leaf: ?CLandscapeBackgroundPatch_ScalarDeletingDestructor@@YIPAXPAXHI@Z
extern void __fastcall AutoTinyScalarDestructor_0065688f(void* self);
void* __fastcall CLandscapeBackgroundPatch_ScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyScalarDestructor_0065688f(self);
    if (flags & 1)
        operator delete(self);
    return self;
}