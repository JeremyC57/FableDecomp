
struct CMember { void Copy_00461bb7(const CMember* src); };
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
// def vtable 0x01232F3C: slot 0 0x00454A64 (implicit inline dtor); real overrides from the def vtable scan
class CCombatAbilityBlockHeavyWeaponAttackDef : public CDefClassBase { public:
    unsigned char _pad[0x48];
    virtual void Transfer(CPersistContext& context);         // slot 18 0x00465C54
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x00461D0D
    virtual unsigned long GetSizeofClass();                  // slot 20 0x00453EB4
};

void __fastcall CCombatAbilityBlockHeavyWeaponAttackDef_Copy(CCombatAbilityBlockHeavyWeaponAttackDef* self, void* edx, const CCombatAbilityBlockHeavyWeaponAttackDef* src)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)src);
    CMember* m = (CMember*)((unsigned char*)self + 0x28);
    m->Copy_00461bb7((const CMember*)((const unsigned char*)src + 0x28));
}