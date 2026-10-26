#pragma optimize("s", on)
extern void __fastcall CUILocaleGraphicsDef_Destruct(void* def);
void* __fastcall CUILocaleGraphicsDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CUILocaleGraphicsDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}