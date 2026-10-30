// CGameScriptInterface::SetThingAsConscious(const CScriptThing&, bool, const CCharString&) const -- knock a live creature out (fall-stunned, with an animation) or bring it round (recover from stunned)
// Leaf: ?SetThingAsConscious@CGameScriptInterface@@UBEXABVCScriptThing@@_NABVCCharString@@@Z
// Retail: Fable.exe @ 0x008A9610, 162 bytes, `ret 0Ch`; Ego_r.pdb same symbol.
// Body: if thing.IsAlive() (CScriptThing slot 75, +0x12c) and its thing (slot 11, +0x2c) is non-null, not dead
// (+0x91 bit 0), a creature (ThingTypeFlags +0x6c bit 3) holding interface 0x31 (UsingComponents[1] bit 17, +0x24):
// conscious -> SetCurrentAction(CCombatAction_RecoverFromStunned(creature, 0)) (ctor 0x008AE1B0, vtable 0x0127851C, RTTI
// .?AVCCombatAction_RecoverFromStunned@@); else SetCurrentAction(CCombatAction_FallStunned(creature, 0, anim)) (ctor
// 0x008EDA90, Ego_r.pdb ??0CCombatAction_FallStunned@@QAE@AAVCThingCreatureBase@@PAV1@ABVCCharString@@@Z);
// CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) 0x006644F0; both temporaries die through
// ~CCombatActionBase 0x0062E900 (the derived destructors are implicit).
// Layout: CThing retail (UsingComponents +0x20, ThingTypeFlags +0x6c, Dead +0x91; PDB +0x7c / +0xa1). The actions are
// Ego_r.pdb 300 / 304 bytes; retail's shared temporary slot is 0x118, so the model gives CCombatActionBase 0x114 bytes and
// RecoverFromStunned its 4 extra bytes. CScriptThing is declared locally with its virtuals (engine/CScriptThing.h
// flattens the vptr).
// Shape: /O2 (16-aligned entry).
class CCharString;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    unsigned char _pad44[0x6c - 0x44];
    unsigned long ThingTypeFlags;                        // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
};
class CThingCreatureBase;
class CCombatActionBase : public CCreatureActionBase {
public:
    CCombatActionBase(CThingCreatureBase& creature, CThingCreatureBase* other);
    virtual ~CCombatActionBase();                        // 0x0062E900
    unsigned char _pad04[0x114 - 4];
};
class CCombatAction_RecoverFromStunned : public CCombatActionBase {
public:
    CCombatAction_RecoverFromStunned(CThingCreatureBase& creature, CThingCreatureBase* other);   // 0x008AE1B0
    unsigned long _pad114;
};
class CCombatAction_FallStunned : public CCombatActionBase {
public:
    CCombatAction_FallStunned(CThingCreatureBase& creature, CThingCreatureBase* other, const CCharString& anim); // 0x008EDA90
};
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);    // 0x006644F0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetThingAsConscious(const CScriptThing& thing, bool conscious, const CCharString& anim) const
{
    if (thing.IsAlive())
    {
        CThing* t = thing.GetPThing();
        if (t && !t->IsDead() && (t->ThingTypeFlags & 8) && t->HasTC(0x31))
        {
            CThingCreatureBase* creature = (CThingCreatureBase*)t;
            if (conscious)
                creature->SetCurrentAction(CCombatAction_RecoverFromStunned(*creature, 0));
            else
                creature->SetCurrentAction(CCombatAction_FallStunned(*creature, 0, anim));
        }
    }
}