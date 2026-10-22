#pragma optimize("s", on)
extern void __fastcall CScorpionKingBattleDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CScorpionKingBattleDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CScorpionKingBattleDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}