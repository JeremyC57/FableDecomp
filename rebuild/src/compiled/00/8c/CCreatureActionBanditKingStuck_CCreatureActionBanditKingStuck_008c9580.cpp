// CCreatureAction_BanditKingStuck::~CCreatureAction_BanditKingStuck() -- compiler-generated destructor: PObstruction, then PStuckEffect (both ~CBaseIntelligentPointer), then ~CCombatActionBase by tail jump
// Leaf: ??1CCreatureAction_BanditKingStuck@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C9580, 33 bytes: `push esi ; mov esi,ecx ; lea ecx,[esi+120h] ; call 0x00A01C10 ;
//   lea ecx,[esi+118h] ; call 0x00A01C10 ; mov ecx,esi ; pop esi ; jmp 0x0062E900`. No caller (uses inlined; the linker
//   keeps unreferenced COMDATs).
// Identity (proven): it follows CCreatureAction_BanditKingStuck's members in its TU (constructor 0x008C94D0, landed, stores
//   vtable 0x01281C54; GetAnimationTransitionInTime 0x008C9560; IsOverrideableByActionOfSamePriority 0x008C9570), and that
//   class's scalar deleting destructor 0x008C9690 (vtable slot 0) inlines the same sequence (+0x120, +0x118, 0x0062E900); no
//   other action vtable's slot 0 destroys +0x120/+0x118. Ego_r.pdb CCreatureAction_BanditKingStuck : CCombatActionBase has
//   TimesToLoop +0x12C, PStuckEffect +0x130, PObstruction +0x138 (CIntelligentPointer<CThing>); retail CCombatActionBase is
//   0x114, hence +0x114 / +0x118 / +0x120 (the constructor stores both pointers there). FableWin.pdb names
//   ??1CCreatureAction_BanditKingStuck@@UAE@XZ (compiler-generated: no vtable store). Catalog label `_Dest_val<...>` propagated.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10, CCombatActionBase::~CCombatActionBase 0x0062E900 (landed).
// Layout: retail CCombatActionBase is 0x114 (landed ~CCombatActionBase 0x0062E900); engine/CCreatureActionBase.h (and the
//   generated CCombatAction headers) carry PDB sizes with flattened bases, so the bases are modelled locally at retail size.
// Emission: the TU harness below destroys a local action by name (the shape retail's action users have); not a retail function.
class CBaseObject;
class CBaseObjectPointer;
class CThing;
class CThingCreatureBase;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();                         // 0x00693EF0
};
class CCombatActionBase : public CCreatureActionBase {      // retail sizeof 0x114
public:
    virtual ~CCombatActionBase();                           // 0x0062E900
private:
    unsigned char _body[0x114 - 4];
};

class CCreatureAction_BanditKingStuck : public CCombatActionBase {   // vtable 0x01281C54
public:
    CCreatureAction_BanditKingStuck(CThingCreatureBase& creature, CThing& target, long timesToLoop, CThing* stuckEffect,
                                    CThing* obstruction);   // 0x008C94D0
    long                        TimesToLoop;                // +0x114
    CIntelligentPointer<CThing> PStuckEffect;               // +0x118
    CIntelligentPointer<CThing> PObstruction;               // +0x120
};

// TU harness (not a retail function): a local action destroyed by name.
void TUHarness_BanditKingStuck(CThingCreatureBase& creature, CThing& target, long timesToLoop, CThing* stuckEffect,
                               CThing* obstruction)
{
    CCreatureAction_BanditKingStuck action(creature, target, timesToLoop, stuckEffect, obstruction);
}