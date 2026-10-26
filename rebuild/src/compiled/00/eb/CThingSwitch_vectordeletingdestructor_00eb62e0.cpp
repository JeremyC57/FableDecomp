#pragma optimize("t", on)
// Leaf: `vector_deleting_destructor'
extern void __fastcall AutoTinyFirstMemberDestructor_0099eae0(void* member);
extern void __fastcall AutoTinySecondMemberDestructor_007e74d0(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00f35b40(void* self);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyFirstMemberDestructor_0099eae0(static_cast<unsigned char*>(self) + 52);
    AutoTinySecondMemberDestructor_007e74d0(static_cast<unsigned char*>(self) + 28);
    AutoTinyCompositeDestructor_00f35b40(self);
    if (flags & 1)
        operator delete(self);
    return self;
}