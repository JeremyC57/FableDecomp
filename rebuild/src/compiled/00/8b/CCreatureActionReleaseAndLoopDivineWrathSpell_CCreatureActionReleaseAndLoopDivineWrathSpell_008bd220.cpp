// CCreatureAction_ReleaseAndLoopDivineWrathSpell::CCreatureAction_ReleaseAndLoopDivineWrathSpell(CThingCreatureBase*) -- builds the "ST_RELEASE_LOOP_DIVINE_WRATH" name, runs the inlined base CCreatureAction_BuildUpDivineWrathSpell(owner, name) (CCombatActionBase(owner, 150, zeroed CSoundPair), vtable 0x0127E8D4, SetAnimationName), destroys the name, installs this class's vtable (0x0127EB7C)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BD220, length 94 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030, CCombatActionBase::SetAnimationName @ 0x008574D0, CCharString::~CCharString @ 0x0099EAE0.
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
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

static inline CCharString AnimationName() { return CCharString("ST_RELEASE_LOOP_DIVINE_WRATH", -1); }   // the name built by value before anything else
class CCreatureAction_BuildUpDivineWrathSpell : public CCombatActionBase { public:   // vtable 0x0127E8D4: the build-up action takes its animation name from the derived class
    virtual void v00();
    CCreatureAction_BuildUpDivineWrathSpell(CThingCreatureBase* owner, const CCharString& name)
        : CCombatActionBase(owner, 150, CSoundPair())
    {
        SetAnimationName(name);
    }
};

class CCreatureAction_ReleaseAndLoopDivineWrathSpell : public CCreatureAction_BuildUpDivineWrathSpell { public:   // vtable 0x0127EB7C
    virtual void v00();
    CCreatureAction_ReleaseAndLoopDivineWrathSpell(CThingCreatureBase* owner);
};

CCreatureAction_ReleaseAndLoopDivineWrathSpell::CCreatureAction_ReleaseAndLoopDivineWrathSpell(CThingCreatureBase* owner)
    : CCreatureAction_BuildUpDivineWrathSpell(owner, AnimationName())
{
}