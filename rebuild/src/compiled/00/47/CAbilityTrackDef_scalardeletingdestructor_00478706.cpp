#pragma optimize("s", on)
extern void __fastcall CAbilityTrackDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CAbilityTrackDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CAbilityTrackDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}