#pragma optimize("s",on)
void operator delete(void*);
void __fastcall CSpecialAbilitiesBattleChargeDef_dtor(void* def);

void* __fastcall CSpecialAbilitiesBattleChargeDef_scalar_deleting_destructor(void* def, int edx, unsigned int flags)
{
    CSpecialAbilitiesBattleChargeDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}