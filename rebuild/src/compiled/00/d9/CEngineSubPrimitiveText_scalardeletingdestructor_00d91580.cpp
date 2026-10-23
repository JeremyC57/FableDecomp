#pragma optimize("t", on)
extern void __fastcall AutoTinyMemberDestructor(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00f35b40(void* self);
extern void __cdecl AutoTinyCompositeDelete(void* object);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyMemberDestructor(static_cast<unsigned char*>(self) + 40);
    AutoTinyCompositeDestructor_00f35b40(self);
    if (flags & 1)
        AutoTinyCompositeDelete(self);
    return self;
}