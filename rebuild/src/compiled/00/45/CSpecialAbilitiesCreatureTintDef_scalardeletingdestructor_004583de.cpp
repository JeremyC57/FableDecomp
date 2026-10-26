#pragma optimize("s", on)
extern void __fastcall CSpecialAbilitiesCreatureTintDef_Destruct(void* def);
void* __fastcall CSpecialAbilitiesCreatureTintDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CSpecialAbilitiesCreatureTintDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}