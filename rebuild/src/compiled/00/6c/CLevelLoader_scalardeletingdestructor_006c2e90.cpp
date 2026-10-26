#pragma optimize("t", on)
extern void __fastcall DestructMember_006c2d20(void* member);
extern void __fastcall CLevelLoader_Destruct(void* loader);
void* __fastcall CLevelLoader_CompositeScalarDeletingDestructor(void* loader, int, unsigned int flags)
{
    DestructMember_006c2d20(static_cast<unsigned char*>(loader) + 20);
    CLevelLoader_Destruct(loader);
    if (flags & 1)
        operator delete(loader);
    return loader;
}