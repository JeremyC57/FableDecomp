// NCreatureMode::CModeScreamerAggressive::~CModeScreamerAggressive() -- implicit destructor: PTarget, PEffect, then the Filter (its Group string and the CAnimFilterBase vtable reset), then ~CModeStandBase
// Leaf: ??1CModeScreamerAggressive@NCreatureMode@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00741390, 42 bytes, thiscall; tail `jmp ~CModeStandBase` 0x0073D860.
// Identity (proven, two sources): the only caller is the scalar deleting destructor 0x00741370, slot 0 of vtable 0x012681EC
//   (RTTI .?AVCModeScreamerAggressive@NCreatureMode@@ : CModeStandBase : CTCCreatureModeManager::CModeBase; the vtable is stored by the
//   constructor at 0x007412CB / 0x0096DD2B); FableWin.pdb ??1CModeScreamerAggressive@NCreatureMode@@UAE@XZ (0x024BD9C0). It stores no vtable of its own, so it is
//   the compiler-generated destructor. Members (Ego_r.pdb, no STL container, so the offsets are retail's): Filter
//   (CAnimFilterOfGroup) +0x68, PEffect (CIntelligentPointer<CThing>) +0x74, PTarget (CIntelligentPointer<CThing>) +0x7C; sizeof 0x84.
//   The catalog label "_Dest_val<std::allocator<CMacro>,CMacro>" is propagated and wrong. Byte-near-twins 0x00740420 (~CModeJackOfBladesFloat) / 0x008AC4D0 (~CModeScreamerSuperAttack) are
//   other classes' implicit destructors (each named from its own ??_G / vtable; three symbols).
// Callees: ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10 (twice), ??1CCharString@@QAE@XZ 0x0099EAE0, ??_7CAnimFilterBase@@6B@
//   0x0122FFCC (the inline ~CAnimFilterBase store), ??1CModeStandBase@@UAE@XZ 0x0073D860 (tail).
// Shape: members destroyed in reverse declaration order; the implicit ~CAnimFilterOfGroup and the inline `virtual ~CAnimFilterBase() {}`
//   expand here. Defining retail's own constructor (??0CModeScreamerAggressive@NCreatureMode@@QAE@AAVCThing@@@Z (also a (CThing&, CThing&) overload); body not reconstructed here, only its
//   member-init order) emits the vtable, hence ??_G and this out-of-line ??1 -- no emission helper.
// Layout (NF247, q12682 header port): engine/NCreatureMode_CModeScreamerAggressive.h (Ego_r.pdb layout; its FrameUpdate / OnActivate /
//   OnDeactivate overrides plus the NF247 opt-in FABLE_RETAIL_NCREATUREMODE_CMODESCREAMERAGGRESSIVE_VTABLE: the ctor, GetType and GetAnimFilter),
//   on CTCCreatureModeManager.h (FABLE_RETAIL_CMODEBASE_VIRTUALS), CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE + PASS22_VTABLE),
//   CAnimFilterBase.h / CAnimFilterOfGroup.h (INLINE_CTOR: the implicit, inline ~CAnimFilterOfGroup) and CIntelligentPointer.h.
// Vtable 0x012681EC (NF247, q12682): slots 3 / 4 / 5 / 6 now bind the class's own overrides (retail 0x008DA610 / 0x00741360 / 0x008DA410 /
//   0x008DA690; Ego_r.pdb / FableWin.pdb / Ego_d.pdb), not the CModeBase names.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE                  // NF242 (q12677): CModeStandBase's slot 2 / 7 / 9 overrides (0x00862EC0 / 0x00861900 / 0x0073DA00)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING                            // GetAsCharArray / GetLength for the inline CAnimFilterOfGroup ctor
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#define FABLE_RETAIL_NCREATUREMODE_CMODESCREAMERAGGRESSIVE_VTABLE
#include "fable_string.h"                                   // CCharString (~CCharString 0x0099EAE0)
#include "engine/ETCInterfaceType.h"
#include "engine/NCreatureMode_CModeScreamerAggressive.h"

NCreatureMode::CModeScreamerAggressive::CModeScreamerAggressive(CThing& thing)
    : CModeStandBase(thing), Filter(CCharString())
{
}
