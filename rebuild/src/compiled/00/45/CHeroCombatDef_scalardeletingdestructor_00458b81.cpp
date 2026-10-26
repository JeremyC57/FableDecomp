#pragma optimize("s", on)
extern void __fastcall CHeroCombatDef_Destruct(void* def);
void* __fastcall CHeroCombatDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CHeroCombatDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}