// CCountedPointer::~CCountedPointer() -- the destructor: ReleaseData() expanded (speed TU) (T = CCombatActionBase::CStrikeEvent)
// Leaf: ??1?$CCountedPointer@VCStrikeEvent@CCombatActionBase@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062D9A0, 53 bytes. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Was landed as _global::CCountedPointer_Release_0062d9a0 (stand-in name).
// T = CCombatActionBase::CStrikeEvent, proven by: CCombatActionBase::SetupCombatStrikeEvents 0x00859A70 (/O2) releases a local CCountedPointer<CCombatActionBase::CStrikeEvent> here after pushing it into StrikeEvents (Ego_r list<CCountedPointer<CCombatActionBase::CStrikeEvent>>); the landed caller binds ~CCountedPointer<CCombatActionBase::CStrikeEvent>; FableWin.pdb ??1 name.
// Member: the destructor. Retail's ~CCountedPointer is `{ ReleaseData(); }`; in a speed TU VC7.1 expands ReleaseData into it
//   (53 bytes, Data zeroed before Info) and /O2 callers (scalar deleting dtors, scope ends past the inline budget) call ??1
//   (probe: /O2 scalar deleting dtor calls ??1, /O1 calls ReleaseData).
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (destructor = ReleaseData(); ReleaseData zeroes Data first).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_STRIKEEVENT
#include "engine/CCombatActionBase.h"
template CCountedPointer<CCombatActionBase::CStrikeEvent>::~CCountedPointer();
