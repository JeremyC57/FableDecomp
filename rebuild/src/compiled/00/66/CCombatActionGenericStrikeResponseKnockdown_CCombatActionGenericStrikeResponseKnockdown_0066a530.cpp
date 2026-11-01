// CCombatAction_GenericStrikeResponseKnockdown::~CCombatAction_GenericStrikeResponseKnockdown() -- compiler-generated destructor: PThingThatHit (~CBaseIntelligentPointer), then AnimName (~CCharString), then ~CCombatActionBase by tail jump
// Leaf: ??1CCombatAction_GenericStrikeResponseKnockdown@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066A530, 33 bytes: `push esi ; mov esi,ecx ; lea ecx,[esi+118h] ; call 0x00A01C10 ;
//   lea ecx,[esi+114h] ; call 0x0099EAE0 ; mov ecx,esi ; pop esi ; jmp 0x0062E900`.
// Identity (proven): both callers sit in CThingCreatureBase::ReactToGenericStrike (0x00667450; calls at 0x00667A66 and
//   0x00667ABD), each right after building a local with the CCombatAction_GenericStrikeResponseKnockdown constructor
//   0x008679C0 (landed; stores vtable 0x01277344) and passing it to CThingCreatureBase::SetCurrentAction 0x006644F0. That
//   class's scalar deleting destructor 0x00868350 (vtable slot 0) inlines the same sequence (+0x118 0x00A01C10, +0x114
//   0x0099EAE0, 0x0062E900). Ego_r.pdb and FableWin.pdb both name ??1CCombatAction_GenericStrikeResponseKnockdown@@UAE@XZ
//   (an out-of-line copy in the release PDB too; no vtable store: compiler-generated). Catalog label `_Dest_val<...>` propagated.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (the implicit ~CIntelligentPointer<const CThing>
//   inlines to it), CCharString::~CCharString 0x0099EAE0, CCombatActionBase::~CCombatActionBase 0x0062E900 (landed).
// Layout: Ego_r.pdb AnimName (CCharString) +0x12C, PThingThatHit (CIntelligentPointer<const CThing>) +0x130; retail
//   CCombatActionBase is 0x114, hence +0x114 / +0x118 (the landed constructor 0x008679C0). The trailing members (PointOfHit
//   +0x120, GotPointOfHit +0x12C, DirectionOfHit +0x12D, GotDirectionOfHit +0x139, FlipFacing +0x13A: 1-aligned C3DVectors)
//   have trivial destructors and are left out of this model; engine/CCreatureActionBase.h carries the PDB size (0xB4). engine/CCombatAction_GenericStrikeResponseKnockdown.h carries the
//   PDB offsets with a flattened base, so the class is modelled locally.
// Emission: the TU harness below destroys a local action by name as ReactToGenericStrike does after SetCurrentAction; it is not a retail function.
class CBaseObject;
class CBaseObjectPointer;
class CThing;
class CThingCreatureBase;
class C3DVector;

class CCharString {
public:
    ~CCharString();                                         // 0x0099EAE0
private:
    void* PStringData;                                      // +0x00
};

class CBaseIntelligentPointer {                             // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>: vtable 0x01237F1C
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

class CCombatAction_GenericStrikeResponseKnockdown : public CCombatActionBase {   // vtable 0x01277344
public:
    CCombatAction_GenericStrikeResponseKnockdown(CThingCreatureBase& creature, const CThing* thingThatHit,
                                                 const C3DVector& pointOfHit, const C3DVector& directionOfHit);   // 0x008679C0
    CCharString                       AnimName;             // +0x114
    CIntelligentPointer<const CThing> PThingThatHit;        // +0x118
};

// TU harness (not a retail function): a local action destroyed by name, as ReactToGenericStrike destroys its own.
void TUHarness_GenericStrikeResponseKnockdown(CThingCreatureBase& creature, const CThing* thingThatHit,
                                              const C3DVector& pointOfHit, const C3DVector& directionOfHit)
{
    CCombatAction_GenericStrikeResponseKnockdown action(creature, thingThatHit, pointOfHit, directionOfHit);
}