#pragma optimize("s", on)
extern void __fastcall CJackOfBladesBattleDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CJackOfBladesBattleDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CJackOfBladesBattleDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}