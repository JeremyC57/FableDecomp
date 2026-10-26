// CCreatureAction_ReleaseAndLoopUnholyPowerSpell::CCreatureAction_ReleaseAndLoopUnholyPowerSpell(CThingCreatureBase*) -- builds the "ST_RELEASE_LOOP_UNHOLY_POWER" name, runs the inlined base CCreatureAction_BuildUpUnholyPowerSpell(owner, name) (CCombatActionBase(owner, 150, zeroed CSoundPair), vtable 0x0127EE44, SetAnimationName), destroys the name, installs this class's vtable (0x0127F0EC)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BD390, length 94 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030, CCombatActionBase::SetCombatAnimation @ 0x008574D0, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the name comes from an inline helper returning it by value (built into the frame before the base call, address re-derived for SetAnimationName, destroyed after it); the two vtable stores bracket the inlined intermediate body. Class names from rtti-vtables.tsv.
#include "fable_string.h"
class CThingCreatureBase; class CThing;
class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(long actionCode);                                                                         // 0x00857E40
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                     // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);     // 0x00857F30
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

static inline CCharString AnimationName() { return CCharString("ST_RELEASE_LOOP_UNHOLY_POWER", -1); }   // the name built by value before anything else
class CCreatureAction_BuildUpUnholyPowerSpell : public CCombatActionBase { public:   // vtable 0x0127EE44: the build-up action takes its animation name from the derived class
    virtual void v00();
    CCreatureAction_BuildUpUnholyPowerSpell(CThingCreatureBase* owner, const CCharString& name)
        : CCombatActionBase(owner, 150, CSoundPair())
    {
        SetCombatAnimation(name);
    }
};

class CCreatureAction_ReleaseAndLoopUnholyPowerSpell : public CCreatureAction_BuildUpUnholyPowerSpell { public:   // vtable 0x0127F0EC
    virtual void v00();
    CCreatureAction_ReleaseAndLoopUnholyPowerSpell(CThingCreatureBase* owner);
};

CCreatureAction_ReleaseAndLoopUnholyPowerSpell::CCreatureAction_ReleaseAndLoopUnholyPowerSpell(CThingCreatureBase* owner)
    : CCreatureAction_BuildUpUnholyPowerSpell(owner, AnimationName())
{
}