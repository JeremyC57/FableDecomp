#pragma optimize("s", on)
extern void __fastcall CBossDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CBossDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CBossDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}