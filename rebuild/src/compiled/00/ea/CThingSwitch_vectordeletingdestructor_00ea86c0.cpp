#pragma optimize("t", on)
// Leaf: `vector_deleting_destructor'
extern void __fastcall AutoTinyFirstMemberDestructor_007add50(void* member);
extern void __fastcall AutoTinySecondMemberDestructor_004aa840(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyFirstMemberDestructor_007add50(static_cast<unsigned char*>(self) + 88);
    AutoTinySecondMemberDestructor_004aa840(static_cast<unsigned char*>(self) + 76);
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}