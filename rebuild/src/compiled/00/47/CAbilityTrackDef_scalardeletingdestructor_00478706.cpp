#pragma optimize("s", on)
extern void __fastcall CAbilityTrackDef_Destruct(void* def);
void* __fastcall CAbilityTrackDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CAbilityTrackDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}