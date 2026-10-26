#pragma optimize("t", on)
// Leaf: `scalar_deleting_destructor'
extern void __fastcall AutoTinyMemberDestructor_008ac970(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyMemberDestructor_008ac970(static_cast<unsigned char*>(self) + 72);
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}