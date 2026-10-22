#pragma optimize("s", on)
extern void __fastcall CTrophyDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTrophyDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CTrophyDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}