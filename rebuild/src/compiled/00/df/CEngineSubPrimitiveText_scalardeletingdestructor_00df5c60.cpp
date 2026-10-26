#pragma optimize("t", on)
// Leaf: `scalar_deleting_destructor'
extern void __fastcall AutoTinyMemberDestructor(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyMemberDestructor(static_cast<unsigned char*>(self) + 76);
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}