// NCreatureMode::CModeBasic::Alloc(CThing& owner) -- mode factory: new CModeBasic(owner) with the inline constructor expanded (the CModeBase
// constructor 0x0073EAC0, then this class's vtable 0x012675BC stored)
// Leaf: ?Alloc@CModeBasic@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00742460, 44 bytes, static __fastcall (ecx = owner).
// Retail bytes:  push esi ; push edi ; push 0x1c ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call 0x0073EAC0 ; pop edi ; mov dword ptr [esi],0x012675BC ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// NF39 (2026-11-16): moved off its `CModeBasic::Clone()` stand-in onto the PDB spelling and the shared engine/CTCCreatureModeManager.h (CModeBase,
//   opt-in FABLE_RETAIL_CMODEBASE_CTOR for ??0CModeBase@CTCCreatureModeManager@@QAE@AAVCThing@@@Z, the only CModeBase constructor in Ego_r.pdb /
//   FableWin.pdb, so 0x0073EAC0 is not a copy constructor). Identity: Ego_r.pdb (0x024B5860) + FableWin.pdb ?Alloc@CModeBasic@NCreatureMode@@SI...;
//   the only reference to 0x00742460 is the data store at 0x0073CA54 in CTCCreatureModeManager::RegisterModes (CRegisteredModeInfo for MODE_BASIC,
//   landed 0x0073B130, already bound), never a vtable slot, so it is no virtual Clone; neither PDB has a Clone for CModeBasic or CModeBase.
// NCreatureMode::CModeBasic has no shared header: its slot-1 override GetType (0x0073EBB0, landed) and the factory are declared here; it adds no data
//   (sizeof 0x1C = sizeof(CModeBase)); its inline constructor is the expanded one above.
// Callees: operator new 0x00BFEA1A, CTCCreatureModeManager::CModeBase::CModeBase(CThing&) 0x0073EAC0.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMODEBASE_CTOR
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"

namespace NCreatureMode {
class CModeBasic : public CTCCreatureModeManager::CModeBase {
public:
    CModeBasic(CThing& owner) : CTCCreatureModeManager::CModeBase(owner) { }
    virtual unsigned long GetType() const;                                    // 0x0073EBB0
    static CTCCreatureModeManager::CModeBase* __fastcall Alloc(CThing& owner);
};

CTCCreatureModeManager::CModeBase* __fastcall CModeBasic::Alloc(CThing& owner)
{
    return new CModeBasic(owner);
}
}
