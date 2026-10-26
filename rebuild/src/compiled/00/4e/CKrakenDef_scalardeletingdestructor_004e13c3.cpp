#pragma optimize("s", on)
extern void __fastcall CKrakenDef_Destruct(void* def);
void* __fastcall CKrakenDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CKrakenDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}