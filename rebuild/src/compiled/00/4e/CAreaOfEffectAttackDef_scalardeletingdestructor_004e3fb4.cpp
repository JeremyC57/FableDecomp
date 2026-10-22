#pragma optimize("s", on)
extern void __fastcall CAreaOfEffectAttackDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CAreaOfEffectAttackDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CAreaOfEffectAttackDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}