// CCreatureAction_InterruptableNearAttack::CCreatureAction_InterruptableNearAttack(CThingCreatureBase& owner, CThing& target) -- the combat-action base for owner and target with action code 150 and an empty sound pair, then this class's vtable
// Leaf: ??0CCreatureAction_InterruptableNearAttack@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00906BA0, 58 bytes, thiscall, `ret 8`, returns this. Vtable 0x01262D94.
// Identity (proven): it stores vtable 0x01262D94 whose RTTI is .?AVCCreatureAction_InterruptableNearAttack@@ (bases CCombatActionBase : CCreatureActionBase :
//   CBaseObject : CBaseClass, ghidra_out/rtti_bases.tsv); FableWin.pdb ??0CCreatureAction_InterruptableNearAttack@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z (donor 0x02B3F570);
//   no direct callers (an out-of-line inline COMDAT).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase&, CThing&, long, const CSoundPair&) 0x00857F30.
// Shape (family K036, representative 0x008AE160): the CSoundPair() temporary's two zero words before the pushes, the base
//   call, the vtable store -- the landed CCreatureAction_CastBattleCharge 0x008CE290 base call without derived members.
class CThing { };
class CThingCreatureBase : public CThing { };
class CSoundPair {                                       // Ego_r.pdb: two sound ids, zeroed by the inline constructor
public:
    CSoundPair() : FirstSound(0), LastSound(0) { }
    long FirstSound;
    long LastSound;
};
class CCombatActionBase {                                // retail sizeof 0x114 (Ego_r.pdb 0x12C: wider containers there)
public:
    CCombatActionBase(CThingCreatureBase& owner, CThing& target, long actionCode, const CSoundPair& sound);   // 0x00857F30
    virtual ~CCombatActionBase();
    unsigned char _body[0x114 - 4];
};
class CCreatureAction_InterruptableNearAttack : public CCombatActionBase {
public:
    CCreatureAction_InterruptableNearAttack(CThingCreatureBase& owner, CThing& target);
    virtual ~CCreatureAction_InterruptableNearAttack();
};
CCreatureAction_InterruptableNearAttack::CCreatureAction_InterruptableNearAttack(CThingCreatureBase& owner, CThing& target)
    : CCombatActionBase(owner, target, 150, CSoundPair())
{
}