#pragma optimize("t", on)
// Leaf: `vector_deleting_destructor'
extern void __fastcall AutoTinyFirstMemberDestructor(void* member);
extern void __fastcall AutoTinySecondMemberDestructor(void* member);
extern void __fastcall AutoTinyCompositeDestructor(void* self);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyFirstMemberDestructor(static_cast<unsigned char*>(self) + 72);
    AutoTinySecondMemberDestructor(static_cast<unsigned char*>(self) + 52);
    AutoTinyCompositeDestructor(self);
    if (flags & 1)
        operator delete(self);
    return self;
}