// CCreatureAction_ShowTrophy::~CCreatureAction_ShowTrophy() -- compiler-generated destructor: PEmitter (+0xB5), then PTrophyBeingShown (+0xA8), then ~CCreatureActionBase by tail jump
// Leaf: ??1CCreatureAction_ShowTrophy@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006DD690, 33 bytes: `push esi ; mov esi,ecx ; lea ecx,[esi+0B5h] ; call 0x00A01C10 ;
//   lea ecx,[esi+0A8h] ; call 0x00A01C10 ; mov ecx,esi ; pop esi ; jmp 0x00693EF0`. No caller (uses inlined; the linker
//   keeps unreferenced COMDATs).
// Identity (proven): the unaligned +0xB5 intelligent pointer is CCreatureAction_ShowTrophy's PEmitter (Ego_r.pdb: PTrophyBeingShown
//   +0xB4, ShowingTrophyDeedHandle +0xBC, PostedDeed +0xC0, PEmitter +0xC1, i.e. retail +0xA8 / +0xB0 / +0xB4 / +0xB5, as the
//   landed constructor 0x006DF730 stores them); of all action scalar deleting destructors only ShowTrophy's 0x006DF7C0 and
//   ShowToSheatheTrophy's 0x006DF8C0 (RTTI: CCreatureAction_ShowToSheatheTrophy : CCreatureAction_ShowTrophy, no members of its
//   own) destroy +0xB5/+0xA8. Retail holds the two implicit destructors back to back, 0x006DD690 and 0x006DD6C0 (landed as
//   `CAggregate_006dd6c0`), in the trophy TU whose constructors / deleting destructors run ShowTrophy first (0x006DF730 /
//   0x006DF7C0, then the ShowToSheatheTrophy ctor 0x006DF890 / 0x006DF8C0); the lane's VC7.1 emits the base class's implicit destructor before the
//   derived class's whichever order the TU uses them in, so the first is ShowTrophy's. FableWin.pdb names
//   ??1CCreatureAction_ShowTrophy@@UAE@XZ (compiler-generated: no vtable store). Catalog label `_Dest_val<...>` propagated.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10, CCreatureActionBase::~CCreatureActionBase 0x00693EF0.
// Layout: retail CCreatureActionBase is 0xA8 (engine/CCreatureActionBase.h carries the 0xB4 PDB size, so the base is local).
//   PEmitter directly after the bool needs the intelligent-pointer classes packed to 1 (the PDB offsets show the same packing;
//   GOTCHAS chunk 196: pack(1) on the pointer classes, not on the action class).
// Emission: the TU harness below destroys a local action by name (the shape retail's action users have); not a retail function.
class CBaseObject;
class CBaseObjectPointer;
class CThing;
class CThingCreatureBase;
class CCharString;

#pragma pack(push, 1)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)

class CCreatureActionBase {                                 // retail sizeof 0xA8
public:
    virtual ~CCreatureActionBase();                         // 0x00693EF0
private:
    unsigned char _body[0xa8 - 4];
};

class CCreatureAction_ShowTrophy : public CCreatureActionBase {   // vtable 0x0125F7B4, retail sizeof 0xBD
public:
    CCreatureAction_ShowTrophy(CThingCreatureBase& creature, CThing& trophy, const CCharString& animName, long priority);   // 0x006DF730
    CIntelligentPointer<CThing> PTrophyBeingShown;          // +0xA8
    unsigned long               ShowingTrophyDeedHandle;    // +0xB0
    bool                        PostedDeed;                 // +0xB4
    CIntelligentPointer<CThing> PEmitter;                   // +0xB5
};

// TU harness (not a retail function): a local action destroyed by name.
void TUHarness_ShowTrophy(CThingCreatureBase& creature, CThing& trophy, const CCharString& animName, long priority)
{
    CCreatureAction_ShowTrophy action(creature, trophy, animName, priority);
}