
struct CMember { void Copy_00461bb7(const CMember* src); };
class CDefClassBase { public: CDefClassBase& operator=(const CDefClassBase& other); };
class CCombatAbilityBlockHeavyWeaponAttackDef : public CDefClassBase { public:
    unsigned char _pad[0x48];
};

void __fastcall CCombatAbilityBlockHeavyWeaponAttackDef_Copy(CCombatAbilityBlockHeavyWeaponAttackDef* self, void* edx, const CCombatAbilityBlockHeavyWeaponAttackDef* src)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)src);
    CMember* m = (CMember*)((unsigned char*)self + 0x28);
    m->Copy_00461bb7((const CMember*)((const unsigned char*)src + 0x28));
}