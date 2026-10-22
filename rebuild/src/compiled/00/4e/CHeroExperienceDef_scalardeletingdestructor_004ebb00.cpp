#pragma optimize("s", on)
extern void __fastcall CHeroExperienceDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CHeroExperienceDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CHeroExperienceDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}