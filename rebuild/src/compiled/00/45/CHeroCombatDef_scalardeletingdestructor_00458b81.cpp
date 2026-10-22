#pragma optimize("s", on)
extern void __fastcall CHeroCombatDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CHeroCombatDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CHeroCombatDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}