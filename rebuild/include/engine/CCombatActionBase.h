#ifndef FABLE_ENGINE_CCOMBATACTIONBASE_H
#define FABLE_ENGINE_CCOMBATACTIONBASE_H
#include "engine/CCreatureActionBase.h"
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
// Retail opt-in (chunk 796, row 0x00951FB0 CAIStateGroup_JackDragon::UpdateHover): Ego_r.pdb
// ??0CCombatActionBase@@QAE@AAVCThingCreatureBase@@JABVCSoundPair@@@Z (out of line, retail 0x00858030; the inline
// CCreatureAction_DragonHoverOutOf(CThingCreatureBase&) ctor calls it with 250 and the "SWORD_SWING" pair) and the protected static
// ?GetSoundPair@CCombatActionBase@@KI?AVCSoundPair@@ABVCThingCreatureBase@@ABVCCharString@@@Z (__fastcall, hidden result, 0x008561A0).
// Declarations only, in both class branches; no layout change.
class CThingCreatureBase; class CSoundPair; class CCharString;
#endif
// Ego_r0x37dee; native base is0x114 after STLport list/vector reductions.
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
// Chunk 764 (row 0x00859660 ResetMeleeCombatAbilityEffects): the Ego_r.pdb members from +0xa8 with retail STLport sizes (Ego_r PTarget
// +0xb4 = retail +0xa8: CCreatureActionBase is 0xc smaller; StrikeEvents list 4 not 0xc, CombatAnimations vector 0xc not 0x10). Row
// evidence: PTarget +0xa8 (IS_THING_ALIVE / GetPItem), PMeleeAbilityDef +0xb8, HasResetMeleeCombatAbilityEffects +0xe0; size 0x114.
// Needs engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER), engine/CDefPointer.h, engine/CCountedPointer.h,
// stlport/fable_containers.h and engine/C3DVector.h first.
class CThing;class CMeleeCombatAbilityDef;class CAnimationEntry;
class CCombatActionBase : public CCreatureActionBase {
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
public: CCombatActionBase(CThingCreatureBase& creature, long priority, const CSoundPair& sound);   // 0x00858030
protected: static CSoundPair __fastcall GetSoundPair(const CThingCreatureBase& creature, const CCharString& name);   // 0x008561A0
#endif
public: virtual ~CCombatActionBase();
    virtual void PerformAction();
protected: virtual CCreatureActionBase* Clone() const;
public: class CStrikeEvent;
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
    // Chunk 764 (row 0x0096DBC0): Ego_r.pdb ?Initialise@CCombatActionBase@@UAEXXZ (0x00857520, called directly by the derived
    // Initialise) and ?SetCombatAnimation@CCombatActionBase@@QAEXABVCCharString@@@Z (0x008574D0).
public:
    virtual void Initialise();                                           // 0x00857520
    void SetCombatAnimation(const CCharString& name);                    // 0x008574D0
#endif
protected:
    void ResetMeleeCombatAbilityEffects();                               // 0x00859660
    CIntelligentPointer<CThing> PTarget;                                 // +0xa8
    CIntelligentPointer<const CThing> PCameraPoint;                      // +0xb0
    CDefPointer<const CMeleeCombatAbilityDef> PMeleeAbilityDef;          // +0xb8
    std::list<CCountedPointer<CStrikeEvent> > StrikeEvents;              // +0xbc
    long LastStrikeEventEndTime;                                         // +0xc0
    float StrikeEventInterruptTime,StrikeEventResetComboTime;            // +0xc4
    long MovementEventInterruptFrame;                                    // +0xcc
    float StrikeSpeedVariationStartTime,StrikeSpeedVariationEndTime;     // +0xd0
    bool HasStrikeEventInterrupt,HasStrikeEventResetCombo,HasMovementEventInterrupt,StrikeEventInterruptActive,
         MovementEventInterruptActive,PreferredFinalPosSet,HasPerformAction,SentMissedEvent,
         HasResetMeleeCombatAbilityEffects;                              // +0xd8 .. +0xe0
    long LastAnimStage;                                                  // +0xe4
    bool HitThing,HitCreature;                                           // +0xe8
    long FrameUpdatePauseFrames;                                         // +0xec
    std::vector<const CAnimationEntry*> CombatAnimations;                // +0xf0
    long ActiveStrikeEventIndex;                                         // +0xfc
    CIntelligentPointer<CThing> LastWeaponUsedInStrike;                  // +0x100
    C3DVector PreferredMovementVector;                                   // +0x108
};
#else
class CCombatActionBase : public CCreatureActionBase {
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
public: CCombatActionBase(CThingCreatureBase& creature, long priority, const CSoundPair& sound);   // 0x00858030
protected: static CSoundPair __fastcall GetSoundPair(const CThingCreatureBase& creature, const CCharString& name);   // 0x008561A0
#endif
public: virtual ~CCombatActionBase();
    virtual void PerformAction();
protected: virtual CCreatureActionBase* Clone() const;
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_SETCOMBATANIMATION
    // Nested opt-in (chunk 854, row 0x00924C70: the inline CCreatureAction_KrakenReturnToWater ctor): Ego_r.pdb
    // ?SetCombatAnimation@CCombatActionBase@@QAEXABVCCharString@@@Z (0x008574D0), the declaration the MELEE branch has under _INITIALISE.
public: void SetCombatAnimation(const CCharString& name);              // 0x008574D0
#endif
#ifdef FABLE_RETAIL_CCOMBATACTIONBASE_RECOVER
public:
    virtual void Initialise();
    void SetCombatAnimation(const CCharString&);
#endif
protected: unsigned char _pad_0xa8[0x6c];
};
#endif
FABLE_STATIC_ASSERT(sizeof(CCombatActionBase)==0x114);
#endif
