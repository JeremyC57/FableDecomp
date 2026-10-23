#pragma optimize("t", on)
extern void __fastcall AutoTinyMemberDestructor_0099a2e0(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00bebc50(void* self);
extern void __cdecl AutoTinyCompositeDelete(void* object);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyMemberDestructor_0099a2e0(static_cast<unsigned char*>(self) + 212);
    AutoTinyCompositeDestructor_00bebc50(self);
    if (flags & 1)
        AutoTinyCompositeDelete(self);
    return self;
}