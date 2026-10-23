#pragma optimize("t", on)
extern void __fastcall AutoTinyFirstMemberDestructor_00cd4470(void* member);
extern void __fastcall AutoTinySecondMemberDestructor_008ac970(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
extern void __cdecl AutoTinyCompositeDelete(void* object);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyFirstMemberDestructor_00cd4470(static_cast<unsigned char*>(self) + 108);
    AutoTinySecondMemberDestructor_008ac970(static_cast<unsigned char*>(self) + 72);
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        AutoTinyCompositeDelete(self);
    return self;
}