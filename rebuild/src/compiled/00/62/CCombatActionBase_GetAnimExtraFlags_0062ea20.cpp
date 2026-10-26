typedef unsigned long ulong;
class CCombatActionBase { public: void* vtbl; };
ulong __fastcall CCombatActionBase_GetAnimExtraFlags(const CCombatActionBase* action)
{
    (void)action;
    return 8u;
}