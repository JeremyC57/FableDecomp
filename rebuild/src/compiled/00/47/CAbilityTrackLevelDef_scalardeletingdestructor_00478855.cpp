#pragma optimize("s", on)
extern void __fastcall CAbilityTrackLevelDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CAbilityTrackLevelDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CAbilityTrackLevelDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}