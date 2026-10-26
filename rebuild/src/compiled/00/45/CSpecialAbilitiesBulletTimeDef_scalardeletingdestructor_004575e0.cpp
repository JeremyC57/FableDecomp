#pragma optimize("s",on)
class CSpecialAbilitiesBulletTimeDef { public: int f0; };
extern void __fastcall CSpecialAbilitiesBulletTimeDef_dtor(CSpecialAbilitiesBulletTimeDef* def);
extern void __cdecl eng_operator_delete(void* p);

void* __fastcall CSpecialAbilitiesBulletTimeDef_scalar_deleting_destructor(CSpecialAbilitiesBulletTimeDef* def, int edx, unsigned int flags) {
    (void)edx;
    CSpecialAbilitiesBulletTimeDef_dtor(def);
    if (flags & 1) {
        eng_operator_delete(def);
    }
    return def;
}