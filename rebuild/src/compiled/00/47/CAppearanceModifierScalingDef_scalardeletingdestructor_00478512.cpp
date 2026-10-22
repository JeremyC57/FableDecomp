#pragma optimize("s", on)
extern void __fastcall CAppearanceModifierScalingDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CAppearanceModifierScalingDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CAppearanceModifierScalingDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}