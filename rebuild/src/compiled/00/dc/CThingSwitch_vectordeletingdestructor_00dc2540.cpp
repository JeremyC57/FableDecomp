#pragma optimize("t", on)
// Leaf: `vector_deleting_destructor'
extern void __fastcall AutoTinyFirstMemberDestructor(void* member);
extern void __fastcall AutoTinySecondMemberDestructor_008ac970(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyFirstMemberDestructor(static_cast<unsigned char*>(self) + 100);
    AutoTinySecondMemberDestructor_008ac970(static_cast<unsigned char*>(self) + 76);
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}