#pragma optimize("s", on)
extern void __fastcall CCreatureModeDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CCreatureModeDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CCreatureModeDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}