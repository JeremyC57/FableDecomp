// Byte-exact reconstruction of CCombatAbilityBlockLightWeaponAttackDef::Copy @ 0x00461b98
// __fastcall void Copy(This*, CDefClassBase const*)

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

// Embedded sub-object at +0x28 that has its own Copy(CDefClassBase const*)
struct SubDef {
    char pad[4];
    void Copy(const CDefClassBase* src);
};

// Base class portion
struct BaseDef {
    char pad[4];
    void Copy(const CDefClassBase* src);
};

class CCombatAbilityBlockLightWeaponAttackDef : public BaseDef { public:
    // offset 0x28: embedded sub-def
    char gap[0x28 - sizeof(BaseDef)];
    SubDef sub;
    void Copy(const CDefClassBase* src);
};

void CCombatAbilityBlockLightWeaponAttackDef::Copy(const CDefClassBase* src)
{
    BaseDef::Copy(src);
    this->sub.Copy((const CDefClassBase*)((const char*)src + 0x28));
}