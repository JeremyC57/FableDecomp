#pragma optimize("s", on)
extern void __fastcall CThingShotDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CThingShotDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CThingShotDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}