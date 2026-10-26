#pragma optimize("s",on)
class CCreatureGenerationFamilyDef { public: int f0; };
extern void __fastcall CCreatureGenerationFamilyDef_dtor(CCreatureGenerationFamilyDef* def);

void* __fastcall CCreatureGenerationFamilyDef_scalar_deleting_destructor(CCreatureGenerationFamilyDef* def, int edx_unused, unsigned int flags)
{
    (void)edx_unused;
    CCreatureGenerationFamilyDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}