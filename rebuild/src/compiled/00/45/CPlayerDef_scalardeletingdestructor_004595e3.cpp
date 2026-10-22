#pragma optimize("s", on)
extern void __fastcall CPlayerDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CPlayerDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CPlayerDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}