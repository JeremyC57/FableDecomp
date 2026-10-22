#pragma optimize("s", on)
extern void __fastcall CMazeBattleDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CMazeBattleDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CMazeBattleDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}