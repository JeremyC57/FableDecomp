#pragma optimize("s", on)
extern void __fastcall CHeroMorphDef_Destruct(void* def);
void* __fastcall CHeroMorphDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CHeroMorphDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}