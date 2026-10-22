typedef unsigned long ulong;
struct CCombatActionBase { void* vtbl; };
ulong __fastcall CCombatActionBase_GetAnimExtraFlags(const CCombatActionBase* action)
{
    (void)action;
    return 8u;
}