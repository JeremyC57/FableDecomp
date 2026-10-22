#pragma optimize("s", on)
extern void __fastcall CThunderBattleDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CThunderBattleDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CThunderBattleDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}