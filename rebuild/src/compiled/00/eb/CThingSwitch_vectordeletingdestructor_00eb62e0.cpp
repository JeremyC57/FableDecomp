#pragma optimize("t", on)
extern void __fastcall AutoTinyFirstMemberDestructor_0099eae0(void* member);
extern void __fastcall AutoTinySecondMemberDestructor_007e74d0(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00f35b40(void* self);
extern void __cdecl AutoTinyCompositeDelete(void* object);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyFirstMemberDestructor_0099eae0(static_cast<unsigned char*>(self) + 52);
    AutoTinySecondMemberDestructor_007e74d0(static_cast<unsigned char*>(self) + 28);
    AutoTinyCompositeDestructor_00f35b40(self);
    if (flags & 1)
        AutoTinyCompositeDelete(self);
    return self;
}