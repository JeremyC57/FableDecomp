#pragma optimize("t", on)
extern void __fastcall AutoTinyMemberDestructor_00baa2b0(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00bd9250(void* self);
extern void __cdecl AutoTinyCompositeDelete(void* object);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyMemberDestructor_00baa2b0(static_cast<unsigned char*>(self) + 296);
    AutoTinyCompositeDestructor_00bd9250(self);
    if (flags & 1)
        AutoTinyCompositeDelete(self);
    return self;
}