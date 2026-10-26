#pragma optimize("s",on)
class CCombatAbilityUseProjectileWeaponDef { public:
    unsigned char _tail[0x48];
};

class CPersistContext { public:
    void PersistA(void* p);
    void PersistB(void* p);
    void PersistC_00452b4d(void* p);
};

void __fastcall CCombatAbilityUseProjectileWeaponDef_Transfer(CCombatAbilityUseProjectileWeaponDef* def, void* edx, CPersistContext* ctx)
{
    ctx->PersistA(def->_tail + 0x40);
    ctx->PersistB(def->_tail + 0x44);
    ctx->PersistC_00452b4d(def->_tail + 0x28);
}