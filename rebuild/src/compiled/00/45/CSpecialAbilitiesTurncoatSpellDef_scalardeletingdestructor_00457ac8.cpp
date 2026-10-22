#pragma optimize("s",on)
struct CSpecialAbilitiesTurncoatSpellDef { void* vftable; };

extern void __fastcall CSpecialAbilitiesTurncoatSpellDef_dtor(CSpecialAbilitiesTurncoatSpellDef* def);
extern void __cdecl eng_operator_delete(void* p);

void* __fastcall CSpecialAbilitiesTurncoatSpellDef_scalar_deleting_destructor(CSpecialAbilitiesTurncoatSpellDef* def, int edx, unsigned int flags) {
    (void)edx;
    CSpecialAbilitiesTurncoatSpellDef_dtor(def);
    if (flags & 1) {
        eng_operator_delete(def);
    }
    return def;
}