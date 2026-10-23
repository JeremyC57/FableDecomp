#pragma optimize("t", on)
extern void __fastcall AutoTinyFirstMemberDestructor_007add50(void* member);
extern void __fastcall AutoTinySecondMemberDestructor_004aa840(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
extern void __cdecl AutoTinyCompositeDelete(void* object);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyFirstMemberDestructor_007add50(static_cast<unsigned char*>(self) + 88);
    AutoTinySecondMemberDestructor_004aa840(static_cast<unsigned char*>(self) + 76);
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        AutoTinyCompositeDelete(self);
    return self;
}