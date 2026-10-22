#pragma optimize("s", on)
extern void __fastcall CRumbleDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CRumbleDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CRumbleDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}