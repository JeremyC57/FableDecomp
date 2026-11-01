// CCreatureAction_FireMissileWeapon::~CCreatureAction_FireMissileWeapon() -- implicit destructor: release PSpecialShotDef, destroy PTarget, then ~CCreatureActionBase
// Leaf: ??1CCreatureAction_FireMissileWeapon@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078ADF0, 52 bytes, thiscall: PSpecialShotDef.Release() inline (null test,
//   `dec [ecx+4]`, virtual Delete (slot 1) at zero, Object = 0) on +0xC4, ~CBaseIntelligentPointer 0x00A01C10 on PTarget
//   +0xA8, then `jmp ~CCreatureActionBase` 0x00693EF0.
// Identity: callers are the scalar deleting destructor 0x008D6140 (slot 0 of vtable 0x01285424, RTTI
//   .?AVCCreatureAction_FireMissileWeapon@@, bases CCreatureActionBase : CBaseObject : CBaseClass) and five sites that
//   destroy a stack instance: CTCHeroRangedAttacks::FireRangedWeapon 0x0078A5B3, CAIStateGroup_ApprenticeHero::
//   PracticeSkills 0x0092AB24, CAIStateGroup_SkillPractice::PracticeSkills 0x0092B5FF, the ranged-combat Update 0x0095E207
//   and CAIStateGroup_CreatureTakeCover 0x0097A175. Ego_r.pdb ??1CCreatureAction_FireMissileWeapon@@UAE@XZ (donor
//   0x00791DB0); members from engine/CCreatureAction_FireMissileWeapon.h (retail STLport offsets): PTarget
//   CIntelligentPointer<const CThing> +0xA8, ShotAccuracyPercentage +0xB0, ShotDamageMultiplier +0xB4, ShotBuildupTime
//   +0xB8, Decapitate +0xBC, DelayTimer +0xC0, PSpecialShotDef CDefPointer<const CMeleeCombatAbilityDef> +0xC4. The bound
//   spelling ??1CCreatureAction_FireMissileWeapon@@UAE@XZ is right; the second binding ...@@QAE@XZ (from 0x00979F70) is not
//   (NAME_FIXES). The twin 0x007E7030 (~CTCOracleMinigame) is another function.
// Shape: compiler-generated destructor (no own vtable store); members die in reverse order (CDefPointer's inline Release,
//   engine/CDefPointer.h; CIntelligentPointer's implicit destructor = the base's out-of-line one). An emission helper
//   forces the ??1 COMDAT.
// Layout: engine/CCreatureAction_FireMissileWeapon.h and engine/CCreatureActionBase.h flatten the vptr into a data member,
//   so the polymorphic chain is the landed creature-action local model (CCreatureActionBase 0xA8 bytes, vptr first; e.g.
//   0x008CF190) with the header's member names (header debt, NOTES).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                        // CDefPointer<T>: inline Release (`dec [ecx+4]`, Delete slot 1, Object = 0)

class CThing;
class CBaseObjectPointer;
class CMeleeCombatAbilityDef;

class CBaseIntelligentPointer {                         // vtable 0x0129C95C, 8 bytes
public:
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObjectPointer* PData;                          // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};

class CCreatureActionBase {                             // 0xA8 bytes in retail (vptr first)
public:
    virtual ~CCreatureActionBase();                     // 0x00693EF0
    unsigned char _body[0xA8 - 4];
};

class CCreatureAction_FireMissileWeapon : public CCreatureActionBase {   // vtable 0x01285424
public:
    CIntelligentPointer<const CThing>          PTarget;                 // +0xA8
    float                                      ShotAccuracyPercentage;  // +0xB0
    float                                      ShotDamageMultiplier;    // +0xB4
    float                                      ShotBuildupTime;         // +0xB8
    bool                                       Decapitate;              // +0xBC
    long                                       DelayTimer;              // +0xC0
    CDefPointer<const CMeleeCombatAbilityDef>  PSpecialShotDef;         // +0xC4
};

// Emission helper (not a retail symbol): forces the implicit virtual destructor out of line as its own COMDAT.
void EmitFireMissileWeaponDestructor(CCreatureAction_FireMissileWeapon* action)
{
    action->CCreatureAction_FireMissileWeapon::~CCreatureAction_FireMissileWeapon();
}