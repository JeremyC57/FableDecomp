// CCreatureAction_GiveItemToThing::~CCreatureAction_GiveItemToThing() -- compiler-generated destructor: PItemBeingGiven, then PReceivingThing (both ~CBaseIntelligentPointer), then ~CCreatureActionBase by tail jump
// Leaf: ??1CCreatureAction_GiveItemToThing@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00628EE0, 33 bytes: `push esi ; mov esi,ecx ; lea ecx,[esi+0B0h] ; call 0x00A01C10 ;
//   lea ecx,[esi+0A8h] ; call 0x00A01C10 ; mov ecx,esi ; pop esi ; jmp 0x00693EF0`.
// Identity (proven): both callers build a CCreatureAction_GiveItemToThing local through its constructor 0x0062D9E0 (landed,
//   ??0CCreatureAction_GiveItemToThing@@QAE@AAVCThingCreatureBase@@AAVCThing@@1@Z), hand it to
//   CThingCreatureBase::SetCurrentAction 0x006644F0 and then call this body: CWorld::EASkipCutScene (call at 0x0062A8FF) and
//   CTCCarriedActionUseGiveBeer's use slot 0x007ED2C0 (call at 0x007ED37B). FableWin.pdb names
//   ??1CCreatureAction_GiveItemToThing@@UAE@XZ (Ego_r.pdb has none: compiler-generated, so no vtable store); the class's own
//   scalar deleting destructor 0x0062DA90 (vtable 0x0125919C slot 0) inlines the same member sequence. The catalog label
//   `_Dest_val<...CSymbolMap::CSymbolInfo...>` is propagated across five byte-identical action destructors.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (the members' implicit ~CIntelligentPointer<CThing>
//   inlines to it), CCreatureActionBase::~CCreatureActionBase 0x00693EF0.
// Layout: Ego_r.pdb CCreatureAction_GiveItemToThing : CCreatureActionBase has PReceivingThing / PItemBeingGiven
//   (CIntelligentPointer<CThing>) at +0xB4 / +0xBC; retail CCreatureActionBase is 0xA8 (12-byte STLport vectors, map 0xC),
//   so they sit at +0xA8 / +0xB0 (the landed constructor 0x0062D9E0). engine/CCreatureActionBase.h carries the PDB size,
//   so the base is modelled locally at its retail size.
// Emission: an implicit destructor is emitted where a translation unit destroys the object by name; the TU harness below
//   destroys a local action by name as the retail callers do after SetCurrentAction. It is not a retail function.
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

class CCreatureActionBase {                                 // retail sizeof 0xA8
public:
    virtual ~CCreatureActionBase();                         // 0x00693EF0
private:
    unsigned char _body[0xa8 - 4];
};

class CCreatureAction_GiveItemToThing : public CCreatureActionBase {   // vtable 0x0125919C
public:
    CCreatureAction_GiveItemToThing(CThingCreatureBase& creature, CThing& receivingThing, CThing& itemBeingGiven);   // 0x0062D9E0
    CIntelligentPointer<CThing> PReceivingThing;            // +0xA8
    CIntelligentPointer<CThing> PItemBeingGiven;            // +0xB0
};

// TU harness (not a retail function): a local action destroyed by name, as the retail callers destroy theirs.
void TUHarness_GiveItemToThing(CThingCreatureBase& creature, CThing& receivingThing, CThing& itemBeingGiven)
{
    CCreatureAction_GiveItemToThing action(creature, receivingThing, itemBeingGiven);
}