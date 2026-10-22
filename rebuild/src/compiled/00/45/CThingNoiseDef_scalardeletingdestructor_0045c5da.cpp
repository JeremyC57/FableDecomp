#pragma optimize("s", on)
extern void __fastcall CThingNoiseDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CThingNoiseDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CThingNoiseDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}