#pragma optimize("s", on)
// Leaf: `scalar_deleting_destructor'
extern void __fastcall AutoTinyScalarDestructor_00591f62(void* self);
void* __fastcall CAssoc_ScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyScalarDestructor_00591f62(self);
    if (flags & 1)
        operator delete(self);
    return self;
}