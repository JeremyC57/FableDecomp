#pragma optimize("s", on)
extern void __fastcall CTrophyDef_Destruct(void* def);
void* __fastcall CTrophyDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CTrophyDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}