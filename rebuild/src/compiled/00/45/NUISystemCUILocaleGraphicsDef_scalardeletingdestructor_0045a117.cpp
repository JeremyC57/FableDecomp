#pragma optimize("s", on)
extern void __fastcall CUILocaleGraphicsDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CUILocaleGraphicsDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CUILocaleGraphicsDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}