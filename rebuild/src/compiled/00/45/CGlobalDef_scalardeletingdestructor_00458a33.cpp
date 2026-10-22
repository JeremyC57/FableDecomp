#pragma optimize("s", on)
extern void __fastcall CGlobalDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CGlobalDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CGlobalDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}