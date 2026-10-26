#pragma optimize("s",on)
class CCreatureGenerationFamilyDef { public: int f0; };
extern void __fastcall CCreatureGenerationFamilyDef_dtor(CCreatureGenerationFamilyDef* def);
extern void __cdecl eng_operator_delete(void* p);

void* __fastcall CCreatureGenerationFamilyDef_scalar_deleting_destructor(CCreatureGenerationFamilyDef* def, int edx_unused, unsigned int flags)
{
    (void)edx_unused;
    CCreatureGenerationFamilyDef_dtor(def);
    if (flags & 1)
        eng_operator_delete(def);
    return def;
}