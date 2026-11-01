// CCreatureAction_BanditKingStuck::~CCreatureAction_BanditKingStuck() -- compiler-generated destructor: PObstruction, then PStuckEffect (both ~CBaseIntelligentPointer), then ~CCombatActionBase by tail jump
// Leaf: ??1CCreatureAction_BanditKingStuck@@UAE@XZ
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

class CBaseIntelligentPointer {                             // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>: vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

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