#pragma optimize("s", on)
extern void __fastcall CFishingDef_Destruct(void* def);
void* __fastcall CFishingDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CFishingDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}