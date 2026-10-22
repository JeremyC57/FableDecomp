#pragma optimize("s", on)
extern void __fastcall CKrakenDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CKrakenDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CKrakenDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}