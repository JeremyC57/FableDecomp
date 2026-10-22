#pragma optimize("s", on)
extern void __fastcall CFishingDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CFishingDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CFishingDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}