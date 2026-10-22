#pragma optimize("s", on)
extern void __fastcall CTavernGameSpotTheAdditionDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTavernGameSpotTheAdditionDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CTavernGameSpotTheAdditionDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}