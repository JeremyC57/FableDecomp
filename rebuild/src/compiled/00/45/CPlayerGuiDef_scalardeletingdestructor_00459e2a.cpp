#pragma optimize("s", on)
extern void __fastcall CPlayerGuiDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CPlayerGuiDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CPlayerGuiDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}