#pragma optimize("s", on)
extern void __fastcall CHeroMorphDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CHeroMorphDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CHeroMorphDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}