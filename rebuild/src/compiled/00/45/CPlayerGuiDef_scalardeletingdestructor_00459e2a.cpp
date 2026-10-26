#pragma optimize("s", on)
extern void __fastcall CPlayerGuiDef_Destruct(void* def);
void* __fastcall CPlayerGuiDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CPlayerGuiDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}