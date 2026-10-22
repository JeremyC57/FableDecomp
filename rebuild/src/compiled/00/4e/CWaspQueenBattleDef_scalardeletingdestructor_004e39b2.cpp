#pragma optimize("s", on)
extern void __fastcall CWaspQueenBattleDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CWaspQueenBattleDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CWaspQueenBattleDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}