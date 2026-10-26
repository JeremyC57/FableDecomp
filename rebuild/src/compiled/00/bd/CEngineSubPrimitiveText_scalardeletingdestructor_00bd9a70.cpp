#pragma optimize("t", on)
// Leaf: `scalar_deleting_destructor'
extern void __fastcall AutoTinyMemberDestructor_00baa2b0(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00bd9250(void* self);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyMemberDestructor_00baa2b0(static_cast<unsigned char*>(self) + 336);
    AutoTinyCompositeDestructor_00bd9250(self);
    if (flags & 1)
        operator delete(self);
    return self;
}