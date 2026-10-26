#pragma optimize("t", on)
extern void __fastcall DestructMember_00414ef0(void* member);
extern void __fastcall C3DAnimationBuilder_Destruct(void* builder);
void* __fastcall C3DAnimationBuilder_CompositeScalarDeletingDestructor(void* builder, int, unsigned int flags)
{
    DestructMember_00414ef0(static_cast<unsigned char*>(builder) + 4);
    C3DAnimationBuilder_Destruct(builder);
    if (flags & 1)
        operator delete(builder);
    return builder;
}