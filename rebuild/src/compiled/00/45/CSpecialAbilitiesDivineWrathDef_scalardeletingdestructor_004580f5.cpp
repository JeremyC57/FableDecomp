#pragma optimize("s",on)
class CSpecialAbilitiesDivineWrathDef { public: void* vftable; };
extern void __fastcall CSpecialAbilitiesDivineWrathDef_dtor(CSpecialAbilitiesDivineWrathDef* def);
extern void __cdecl operator_delete_stub(void* p);

void* __fastcall CSpecialAbilitiesDivineWrathDef_scalar_deleting_destructor(CSpecialAbilitiesDivineWrathDef* def, int edx_unused, unsigned int flags)
{
    CSpecialAbilitiesDivineWrathDef_dtor(def);
    if (flags & 1)
        operator_delete_stub(def);
    return def;
}