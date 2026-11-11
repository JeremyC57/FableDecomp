// CGameScriptInterface::EntityDecapitate(const CScriptThing& thing) const -- for a valid thing holding interface 0xd1 and the right flags, make a decapitation action its current action
// Leaf: ?EntityDecapitate@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe @ 0x008A8E40, 98 bytes, `ret 4`; Ego_r.pdb same symbol.
// Body: if thing.IsAlive() (slot 75, +0x12c): t = thing.GetPThing() (slot 11); if t holds interface 0xd1 (bit 17 of
// UsingComponents word 6, +0x38), any of bits 0..3 of the +0x6c flag byte, and bit 1 of the +0xbc flag byte:
// t->SetCurrentAction(<action>(t)) (0x006644F0) on a 0x114-byte temporary built by 0x0066A560, destroyed by
// ~CCombatActionBase 0x0062E900. No null test on t.
// Layout: CThing flag bytes +0x6c / +0x91 and CCombatActionBase as the landed 0x008C92C0 model; UsingComponents +0x20 as
// the matched 0x0089A330 row. Interface 0xd1 (CTCDecapitation is the likely class), the +0xbc flag and the action class
// (ctor 0x0066A560) are not confirmed; the action is modelled as CCreatureAction_Decapitate.
// Shape: /O2 (16-aligned entry).
// Name fix (agentNF7 2026-11-10): SetCurrentAction 0x006644F0 takes const CCreatureActionBase& (Ego_r.pdb); the local CCombatActionBase stand-in (no base) replaced by the shared engine/CCombatActionBase.h, so the temporary's dtor binds ??1CCombatActionBase@@UAE@XZ 0x0062E900.
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    unsigned char _pad44[0x6c - 0x44];
    unsigned char m_flags6c;                    // +0x6c bit 3
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0xbc - 0x92];
    unsigned char m_flagsbc;                    // +0xbc bit 1
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCombatActionBase.h"   // ~CCombatActionBase 0x0062E900 (virtual), base CCreatureActionBase (Ego_r.pdb)
class CCreatureActionBase;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
struct CCreatureAction_Decapitate : CCombatActionBase {       // retail sizeof 0x114 (name not recovered)
    CCreatureAction_Decapitate(CThing* victim);          // 0x0066A560
};
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntityDecapitate(const CScriptThing& thing) const
{
    if (thing.IsAlive())
    {
        CThing* t = thing.GetPThing();
        if (t->HasTC(0xd1) && (t->m_flags6c & 0xf) && (t->m_flagsbc & 2))
        {
            CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(t);
            creature->SetCurrentAction(CCreatureAction_Decapitate(t));
        }
    }
}