#pragma optimize("s", on)
extern void __fastcall CBuyableHouseDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CBuyableHouseDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CBuyableHouseDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}