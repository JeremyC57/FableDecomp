#pragma optimize("s",on)
extern void __fastcall CTextureReplacementDef_dtor(void* def);

void* __fastcall CTextureReplacementDef_scalar_deleting_destructor(void* def, int edx, unsigned int flags)
{
    (void)edx;
    CTextureReplacementDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}