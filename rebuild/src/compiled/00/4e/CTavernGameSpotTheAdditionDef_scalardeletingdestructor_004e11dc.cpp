#pragma optimize("s", on)
extern void __fastcall CTavernGameSpotTheAdditionDef_Destruct(void* def);
void* __fastcall CTavernGameSpotTheAdditionDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CTavernGameSpotTheAdditionDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}