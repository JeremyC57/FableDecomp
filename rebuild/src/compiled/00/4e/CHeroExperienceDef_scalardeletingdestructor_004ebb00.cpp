#pragma optimize("s", on)
extern void __fastcall CHeroExperienceDef_Destruct(void* def);
void* __fastcall CHeroExperienceDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CHeroExperienceDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}