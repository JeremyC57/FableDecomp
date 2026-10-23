#pragma optimize("t", on)
extern void __fastcall AutoTinyFirstMemberDestructor(void* member);
extern void __fastcall AutoTinySecondMemberDestructor_007e74d0(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
extern void __cdecl AutoTinyCompositeDelete(void* object);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyFirstMemberDestructor(static_cast<unsigned char*>(self) + 96);
    AutoTinySecondMemberDestructor_007e74d0(static_cast<unsigned char*>(self) + 80);
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        AutoTinyCompositeDelete(self);
    return self;
}