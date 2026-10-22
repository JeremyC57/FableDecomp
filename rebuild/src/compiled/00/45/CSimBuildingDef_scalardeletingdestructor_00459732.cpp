#pragma optimize("s", on)
extern void __fastcall CSimBuildingDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CSimBuildingDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CSimBuildingDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}