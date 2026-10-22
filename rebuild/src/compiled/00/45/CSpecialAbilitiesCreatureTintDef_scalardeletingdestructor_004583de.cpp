#pragma optimize("s", on)
extern void __fastcall CSpecialAbilitiesCreatureTintDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CSpecialAbilitiesCreatureTintDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CSpecialAbilitiesCreatureTintDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}