#pragma optimize("s",on)
struct CCombatAbilityUseProjectileWeaponDef {
    unsigned char _tail[0x48];
};

struct CPersistContext {
    void PersistA(void* p);
    void PersistB(void* p);
    void PersistC(void* p);
};

void __fastcall CCombatAbilityUseProjectileWeaponDef_Transfer(CCombatAbilityUseProjectileWeaponDef* def, void* edx, CPersistContext* ctx)
{
    ctx->PersistA(def->_tail + 0x40);
    ctx->PersistB(def->_tail + 0x44);
    ctx->PersistC(def->_tail + 0x28);
}