// CGameScriptInterface::EntitySetAttackThingImmediately(const CScriptThing& attacker, const CScriptThing& target, bool, bool) const -- when both things are alive and the attacker carries flag bit 3 of +0x6c, make an attack-thing action its current action
// Leaf: ?EntitySetAttackThingImmediately@CGameScriptInterface@@UBEXABVCScriptThing@@0_N1@Z
// Retail: Fable.exe @ 0x008A8EB0, 102 bytes, `ret 0x10`; Ego_r.pdb same symbol. The two bool arguments are not read.
// Body: a = attacker.GetPThing(), t = target.GetPThing() (CScriptThing slot 11, both fetched first); if a alive (non-null,
// bit 0 of +0x91 clear), t alive, and a's +0x6c bit 3: a->SetCurrentAction(<action>(a, t)) (0x006644F0) on a 0x118-byte
// temporary built by 0x008AE160 (owner, target), destroyed by ~CCombatActionBase 0x0062E900.
// Layout: CThing +0x6c flag byte / +0x91 dead bit and CCombatActionBase (retail 0x114 bytes) as the landed
// CCreatureAction_BanditKingGetHit::FinishAction 0x008C92C0. The action class (ctor 0x008AE160, callers 0x008A8EF6,
// 0x00962FEE, 0x00963C84) is not named; modelled as CCreatureAction_AttackThing.
// Shape: /O2 (16-aligned entry).
class CThing { public:
    unsigned char _pad00[0x6c];
    unsigned char m_flags6c;                    // +0x6c bit 3
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
};
inline bool IS_THING_ALIVE(const CThing* t)
{
    return t != 0 && !(t->m_flags91 & 1);
}
class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[0x114 - 4];
    virtual ~CCombatActionBase();                       // 0x0062E900
};
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};
struct CCreatureAction_AttackThing : CCombatActionBase {      // retail sizeof 0x118 (name not recovered)
    unsigned char _body[4];
    CCreatureAction_AttackThing(CThingCreatureBase* owner, CThing* target);   // 0x008AE160
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntitySetAttackThingImmediately(const CScriptThing& attacker, const CScriptThing& target, bool, bool) const
{
    CThing* a = attacker.GetPThing();
    CThing* t = target.GetPThing();
    if (IS_THING_ALIVE(a) && IS_THING_ALIVE(t) && (a->m_flags6c & 8))
    {
        CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(a);
        creature->SetCurrentAction(CCreatureAction_AttackThing(creature, t));
    }
}