#pragma optimize("t", on)
// Leaf: `vector_deleting_destructor'
extern void __fastcall AutoTinyFirstMemberDestructor_00cd4470(void* member);
extern void __fastcall AutoTinySecondMemberDestructor_007e74d0(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00f35b40(void* self);
extern void __cdecl AutoTinyCompositeDelete(void* object);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyFirstMemberDestructor_00cd4470(static_cast<unsigned char*>(self) + 44);
    AutoTinySecondMemberDestructor_007e74d0(static_cast<unsigned char*>(self) + 28);
    AutoTinyCompositeDestructor_00f35b40(self);
    if (flags & 1)
        AutoTinyCompositeDelete(self);
    return self;
}