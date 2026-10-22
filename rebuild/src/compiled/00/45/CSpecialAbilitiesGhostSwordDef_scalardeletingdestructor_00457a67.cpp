#pragma optimize("s",on)
struct CSpecialAbilitiesGhostSwordDef { void* vftable; };
extern void __fastcall CSpecialAbilitiesGhostSwordDef_dtor(CSpecialAbilitiesGhostSwordDef* def);
extern void operator_delete_stub(void* p);

void* __fastcall CSpecialAbilitiesGhostSwordDef_scalar_deleting_destructor(CSpecialAbilitiesGhostSwordDef* def, int edx, unsigned int flags) {
    (void)edx;
    CSpecialAbilitiesGhostSwordDef_dtor(def);
    if (flags & 1) {
        operator_delete_stub(def);
    }
    return def;
}