#ifndef FABLE_ENGINE_CTROLLBATTLEDEF_H
#define FABLE_ENGINE_CTROLLBATTLEDEF_H
#include "engine/CParentDefClassBase.h"
#ifdef FABLE_RETAIL_CTROLLBATTLEDEF_MEMBERS
class CPersistContext;
#endif
class CTrollBattleDef : public CParentDefClassBase {
public:
#ifdef FABLE_RETAIL_CTROLLBATTLEDEF_MEMBERS
    // Retail opt-in (agentNF4, 2026-11-10, row 0x004E4833 Alloc): the def factory (Ego_r.pdb / FableWin.pdb
    // ?Alloc@CTrollBattleDef@@SIPAVCDefClassBase@@XZ: `new` 0x60 bytes, the inline constructor = CParentDefClassBase() 0x00430370 and
    // vtable 0x01242F0C), and the def's own overrides (both PDBs ?Transfer@, ?Copy@,
    // ?GetSizeofClass@). Declarations only. Needs FABLE_RETAIL_DEF_CLASS_BASE.
    static CDefClassBase* FABLE_FASTCALL Alloc();                       // 0x004E4833
    virtual void Transfer(CPersistContext&);                            // slot 18
    virtual void Copy(const CDefClassBase*);                            // slot 19
    virtual unsigned long GetSizeofClass();                             // slot 20
#endif
 static CCharString FABLE_FASTCALL StatGetTypeName(){return CCharString("CTrollBattleDef");}
 float GroundPoundSpeedMultiplierStart,GroundPoundSpeedMultiplierEnd;
 float AttackPosPredictiveness,PercentageChanceToUseSecondaryShotType;
 long SecondaryShotType;
 float MinLaughDistance,MaxTimeBetweenKnockdownAndBoast;
 long DeathExplosionIndex,FallDownExplosionIndex;
};
#endif
