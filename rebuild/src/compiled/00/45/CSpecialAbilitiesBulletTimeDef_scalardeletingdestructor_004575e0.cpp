#pragma optimize("s",on)
class CSpecialAbilitiesBulletTimeDef { public: int f0; };
extern void __fastcall CSpecialAbilitiesBulletTimeDef_dtor(CSpecialAbilitiesBulletTimeDef* def);

void* __fastcall CSpecialAbilitiesBulletTimeDef_scalar_deleting_destructor(CSpecialAbilitiesBulletTimeDef* def, int edx, unsigned int flags) {
    (void)edx;
    CSpecialAbilitiesBulletTimeDef_dtor(def);
    if (flags & 1) {
        operator delete(def);
    }
    return def;
}