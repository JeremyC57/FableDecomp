#pragma optimize("t", on)
// Leaf: `vector_deleting_destructor'
extern void __fastcall AutoTinyFirstMemberDestructor_00cdb960(void* member);
extern void __fastcall AutoTinySecondMemberDestructor_00cd4470(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyFirstMemberDestructor_00cdb960(static_cast<unsigned char*>(self) + 92);
    AutoTinySecondMemberDestructor_00cd4470(static_cast<unsigned char*>(self) + 88);
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}