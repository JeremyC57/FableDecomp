#pragma optimize("t", on)
// Leaf: `vector_deleting_destructor'
extern void __fastcall AutoTinyFirstMemberDestructor(void* member);
extern void __fastcall AutoTinySecondMemberDestructor_008ac970(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00f35b40(void* self);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyFirstMemberDestructor(static_cast<unsigned char*>(self) + 40);
    AutoTinySecondMemberDestructor_008ac970(static_cast<unsigned char*>(self) + 28);
    AutoTinyCompositeDestructor_00f35b40(self);
    if (flags & 1)
        operator delete(self);
    return self;
}