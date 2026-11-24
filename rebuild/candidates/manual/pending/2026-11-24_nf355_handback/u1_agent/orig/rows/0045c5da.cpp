#pragma optimize("s", on)
extern void __fastcall CThingNoiseDef_Destruct(void* def);
void* __fastcall CThingNoiseDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CThingNoiseDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}