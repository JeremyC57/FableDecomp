#pragma optimize("s", on)
extern void __fastcall CGlobalDef_Destruct(void* def);
void* __fastcall CGlobalDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CGlobalDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}