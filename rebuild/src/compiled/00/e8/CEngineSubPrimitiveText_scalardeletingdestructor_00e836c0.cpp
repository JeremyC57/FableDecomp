#pragma optimize("t", on)
// Leaf: `scalar_deleting_destructor'
extern void __fastcall AutoTinyMemberDestructor_0099eae0(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00f35b40(void* self);
extern void __cdecl AutoTinyCompositeDelete(void* object);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyMemberDestructor_0099eae0(static_cast<unsigned char*>(self) + 36);
    AutoTinyCompositeDestructor_00f35b40(self);
    if (flags & 1)
        AutoTinyCompositeDelete(self);
    return self;
}