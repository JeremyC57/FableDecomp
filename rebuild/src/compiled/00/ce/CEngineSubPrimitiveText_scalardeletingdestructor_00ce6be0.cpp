#pragma optimize("t", on)
// Leaf: `scalar_deleting_destructor'
extern void __fastcall AutoTinyMemberDestructor_0099eae0(void* member);
extern void __fastcall AutoTinyCompositeDestructor(void* self);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyMemberDestructor_0099eae0(static_cast<unsigned char*>(self) + 64);
    AutoTinyCompositeDestructor(self);
    if (flags & 1)
        operator delete(self);
    return self;
}