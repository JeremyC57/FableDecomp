extern void __fastcall AutoTinyMemberDestructor_0099eae0(void* member);
// Leaf: `scalar_deleting_destructor'
extern void __fastcall AutoTinyCompositeDestructor_0099b510(void* self);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyMemberDestructor_0099eae0(static_cast<unsigned char*>(self) + 16);
    AutoTinyCompositeDestructor_0099b510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}