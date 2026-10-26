#pragma optimize("s",on)
class CSpecialAbilitiesTurncoatSpellDef { public: void* vftable; };

extern void __fastcall CSpecialAbilitiesTurncoatSpellDef_dtor(CSpecialAbilitiesTurncoatSpellDef* def);

void* __fastcall CSpecialAbilitiesTurncoatSpellDef_scalar_deleting_destructor(CSpecialAbilitiesTurncoatSpellDef* def, int edx, unsigned int flags) {
    (void)edx;
    CSpecialAbilitiesTurncoatSpellDef_dtor(def);
    if (flags & 1) {
        operator delete(def);
    }
    return def;
}