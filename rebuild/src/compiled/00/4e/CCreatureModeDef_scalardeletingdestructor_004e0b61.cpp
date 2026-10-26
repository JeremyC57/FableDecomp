#pragma optimize("s", on)
extern void __fastcall CCreatureModeDef_Destruct(void* def);
void* __fastcall CCreatureModeDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CCreatureModeDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}