#pragma optimize("t", on)
// Leaf: `scalar_deleting_destructor'
extern void __fastcall AutoTinyMemberDestructor_00412130(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00f35b40(void* self);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyMemberDestructor_00412130(static_cast<unsigned char*>(self) + 28);
    AutoTinyCompositeDestructor_00f35b40(self);
    if (flags & 1)
        operator delete(self);
    return self;
}