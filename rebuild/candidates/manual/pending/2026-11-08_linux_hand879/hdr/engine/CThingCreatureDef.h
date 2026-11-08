#ifndef FABLE_ENGINE_CTHINGCREATUREDEF_H
#define FABLE_ENGINE_CTHINGCREATUREDEF_H
#include "engine/CThingBaseDef.h"
// Original TPI0x14ae8; complete native14c, original flags begin148.
class CThingCreatureDef : public CThingBaseDef {
public:
#ifdef FABLE_RETAIL_CTHINGCREATUREDEF_SEX
    // Retail opt-in (chunk 879, row 0x00502E90): Ego_r.pdb Sex (+0xC4) / CreatureType (+0xC8), retail +0xC0 / +0xC4 (the Ego_r
    // Dinkumware vector in CThingBaseDef's chain is 4 bytes larger; GetSex 0x00661FF0 reads +0xC0, GetCreatureType 0x00662000 +0xC4).
    unsigned char _pad_0x70[0x50];
    long Sex;                                                        // +0xC0 ESex
    long CreatureType;                                               // +0xC4 ECreatureType
    unsigned char _pad_0xc8[0x80];
#else
    unsigned char _pad_0x70[0xd8];
#endif
    bool LeaveDeadCreature:1;
    bool AbleToStrafe:1;
    bool CreateDummyOnDeath:1;
    bool ImmuneToAttack:1;
    bool UseMeleeWeaponAsDefault:1;
    bool UnsheatheWeaponsAutomatically:1;
    bool IsWillable:1;
    bool IsIndestructible:1;
    bool IsInvulnerableDuringGenericResponse:1;
    bool IsPushedByExplosions:1;
    unsigned char _pad_0x14a[2];
};
FABLE_STATIC_ASSERT(sizeof(CThingCreatureDef)==0x14c);
// CThingCreatureBase::PeekDefinition (inline, FableWin-only): defined here when engine/CThingCreatureBase.h came first.
#if defined(FABLE_ENGINE_CTHINGCREATUREBASE_H) && defined(FABLE_RETAIL_CTHINGCREATUREBASE)
inline const CThingCreatureDef& CThingCreatureBase::PeekDefinition() const { return *PDef; }
#endif
#endif
