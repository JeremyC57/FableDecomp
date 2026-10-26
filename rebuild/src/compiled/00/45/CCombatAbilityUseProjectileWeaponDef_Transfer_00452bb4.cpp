#pragma optimize("s",on)
class CCombatAbilityUseProjectileWeaponDef { public:
    unsigned char _tail[0x48];
};

class CPersistContext { public:
    template <class T> void Transfer(T& value);
    void PersistC_00452b4d(void* p);
};

void __fastcall CCombatAbilityUseProjectileWeaponDef_Transfer(CCombatAbilityUseProjectileWeaponDef* def, void* edx, CPersistContext* ctx)
{
    ctx->Transfer(*(bool*)(def->_tail + 0x40));
    ctx->Transfer(*(long*)(def->_tail + 0x44));
    ctx->PersistC_00452b4d(def->_tail + 0x28);
}