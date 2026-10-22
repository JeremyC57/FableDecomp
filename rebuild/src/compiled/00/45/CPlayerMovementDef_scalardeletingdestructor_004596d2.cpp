#pragma optimize("s", on)
extern void __fastcall CPlayerMovementDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CPlayerMovementDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CPlayerMovementDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}