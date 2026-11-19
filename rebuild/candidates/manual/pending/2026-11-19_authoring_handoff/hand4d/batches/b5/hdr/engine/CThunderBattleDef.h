#ifndef FABLE_ENGINE_CTHUNDERBATTLEDEF_H
#define FABLE_ENGINE_CTHUNDERBATTLEDEF_H
// CThunderBattleDef (new, hand4d, row 0x008DD7E0 CModeThunderLightningStorm::UpdateDamage): Ego_d.pdb record 0x2F56A / Ego_r.pdb TPI
// (struct_layouts_egor.tsv): public CParentDefClassBase base, then NumGetHitsBeforeStormAttack, NumMissesBeforeStormAttack,
// LightningDamage, BattleChargeLevel. Retail offsets follow the 0x39-byte retail CParentDefClassBase: the members start at +0x3C and
// UpdateDamage reads LightningDamage at +0x44 (0x008DD9C7). Methods as the PDB lists them (Copy, GetSizeofClass, Alloc,
// StatGetTypeName, Transfer, ctors, virtual dtor, operator=); declarations only.
#include "engine/CParentDefClassBase.h"
class CThunderBattleDef:public CParentDefClassBase{
public:
 virtual void Copy(const CDefClassBase * p0);
 virtual unsigned long GetSizeofClass();
 static CDefClassBase * FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL StatGetTypeName();
 long NumGetHitsBeforeStormAttack;
 long NumMissesBeforeStormAttack;
 float LightningDamage;
 long BattleChargeLevel;
 virtual void Transfer(CPersistContext & p0);
 CThunderBattleDef(const CThunderBattleDef & p0);
 CThunderBattleDef();
 virtual ~CThunderBattleDef();
 CThunderBattleDef & operator=(const CThunderBattleDef & p0);
};
FABLE_STATIC_ASSERT(sizeof(CThunderBattleDef)==0x4c);
FABLE_STATIC_ASSERT(offsetof(CThunderBattleDef,LightningDamage)==0x44);
#endif
