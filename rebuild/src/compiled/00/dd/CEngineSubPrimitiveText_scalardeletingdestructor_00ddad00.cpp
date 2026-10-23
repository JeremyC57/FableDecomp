#pragma optimize("t", on)
extern void __fastcall AutoTinyMemberDestructor_00cd4470(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
extern void __cdecl AutoTinyCompositeDelete(void* object);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyMemberDestructor_00cd4470(static_cast<unsigned char*>(self) + 124);
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        AutoTinyCompositeDelete(self);
    return self;
}