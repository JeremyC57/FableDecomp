#pragma optimize("s", on)
extern void __fastcall CFireheartMinigameDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CFireheartMinigameDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CFireheartMinigameDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}