#pragma optimize("s", on)
extern void __fastcall CBalverineBattleDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CBalverineBattleDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CBalverineBattleDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}