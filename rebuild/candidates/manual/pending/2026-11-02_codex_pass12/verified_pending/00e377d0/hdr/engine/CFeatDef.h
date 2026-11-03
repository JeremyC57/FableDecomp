#ifndef FABLE_ENGINE_CFEATDEF_H
#define FABLE_ENGINE_CFEATDEF_H
// Original Ego_r and FableWin class records; retail member offsets verified by
// CScriptDef construction and reverse array destruction at 00f28f90/00f29530.
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CCharString.h"
#include "stlport/fable_containers.h"
enum EFeatAttackType { FAT_ATTACK_ANY = 0, FAT_ATTACK_SWORD = 1, FAT_ATTACK_BOW = 2, FAT_ATTACK_HANDS = 3, FAT_ATTACK_WILL = 4 };
class CFeatDef : public CSubDefClassBase {
public:
    CFeatDef();
    CFeatDef(const CFeatDef&);
    CFeatDef& operator=(const CFeatDef&);
    virtual ~CFeatDef();
    virtual void Copy(const CDefClassBase*);
    virtual unsigned long GetSizeofClass();
    static CDefClassBase* __fastcall Alloc();
    static CCharString __fastcall StatGetTypeName();
    virtual void Transfer(CPersistContext&);
    CDefString FeatName; // +0x0028
    CDefString Verb; // +0x002c
    float TimeLimit; // +0x0030
    long TargetNumber; // +0x0034
    long GoldReward; // +0x0038
    long XPReward; // +0x003c
    CDefString ItemReward; // +0x0040
    bool NoBlocking; // +0x0044
    EFeatAttackType KN_AttackType; // +0x0048
    bool KN_Perfect; // +0x004c
    long KN_CreatureType; // +0x0050
    CDefString GF_FromRegion; // +0x0054
    CDefString GF_ToRegion; // +0x0058
    bool GF_NoTeleporting; // +0x005c
    CDefString CO_ItemName; // +0x0060
};
FABLE_STATIC_ASSERT(sizeof(CFeatDef) == 0x64);
#endif
