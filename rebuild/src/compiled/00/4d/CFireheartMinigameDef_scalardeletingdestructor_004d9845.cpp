#pragma optimize("s", on)
extern void __fastcall CFireheartMinigameDef_Destruct(void* def);
void* __fastcall CFireheartMinigameDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CFireheartMinigameDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}