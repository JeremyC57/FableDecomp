// NCreatureMode::CModeScreamerSuperAttack::~CModeScreamerSuperAttack() -- implicit destructor: PLightningBeamParticleEmitter, PTarget, then the Filter (its Group string and the CAnimFilterBase vtable reset), then ~CModeStandBase
// Leaf: ??1CModeScreamerSuperAttack@NCreatureMode@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AC4D0, 42 bytes, thiscall; tail `jmp ~CModeStandBase` 0x0073D860.
// Identity (proven, two sources): the only caller is the scalar deleting destructor 0x008AC4B0, slot 0 of vtable 0x01278338
//   (RTTI .?AVCModeScreamerSuperAttack@NCreatureMode@@ : CModeStandBase : CTCCreatureModeManager::CModeBase; the vtable is stored by the
//   constructor at 0x008AC40B); FableWin.pdb ??1CModeScreamerSuperAttack@NCreatureMode@@UAE@XZ (0x02AAC090). It stores no vtable of its own, so it is
//   the compiler-generated destructor. Members (Ego_r.pdb, no STL container, so the offsets are retail's): Filter
//   (CAnimFilterOfGroup) +0x68, PTarget (CIntelligentPointer<const CThing>) +0x74, PLightningBeamParticleEmitter (CIntelligentPointer<CThing>) +0x7C; sizeof 0x84.
//   The catalog label "_Dest_val<std::allocator<CMacro>,CMacro>" is propagated and wrong. Byte-near-twins 0x00740420 (~CModeJackOfBladesFloat) / 0x00741390 (~CModeScreamerAggressive) are
//   other classes' implicit destructors (each named from its own ??_G / vtable; three symbols).
// Callees: ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10 (twice), ??1CCharString@@QAE@XZ 0x0099EAE0, ??_7CAnimFilterBase@@6B@
//   0x0122FFCC (the inline ~CAnimFilterBase store), ??1CModeStandBase@@UAE@XZ 0x0073D860 (tail).
// Shape: members destroyed in reverse declaration order; the implicit ~CAnimFilterOfGroup and the inline `virtual ~CAnimFilterBase() {}`
//   expand here. Defining retail's own constructor (??0CModeScreamerSuperAttack@NCreatureMode@@QAE@AAVCThing@@ABV2@@Z (CThing&, const CThing&); body not reconstructed here, only its
//   member-init order) emits the vtable, hence ??_G and this out-of-line ??1 -- no emission helper.
// Layout (NF247, q12682 header port): engine/NCreatureMode_CModeScreamerSuperAttack.h (Ego_r.pdb layout; FrameUpdate / OnActivate / OnDeactivate,
//   plus the NF247 opt-ins _IMPLICIT_DTOR (no user-declared destructor, as retail) and _GETANIMFILTER (slot 4)), on CTCCreatureModeManager.h
//   (FABLE_RETAIL_CMODEBASE_VIRTUALS), CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE + PASS22_VTABLE), CAnimFilterBase.h /
//   CAnimFilterOfGroup.h (INLINE_CTOR: the implicit, inline ~CAnimFilterOfGroup) and CIntelligentPointer.h.
// Vtable 0x01278338 (NF247, q12682): slots 3 / 4 / 5 / 6 now bind the class's own overrides (retail 0x008E16A0 / 0x008AC4A0 / 0x008D8330 /
//   0x008D79A0; Ego_r.pdb / FableWin.pdb / Ego_d.pdb), not the CModeBase names.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE                  // NF242 (q12677): CModeStandBase's slot 2 / 7 / 9 overrides (0x00862EC0 / 0x00861900 / 0x0073DA00)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING                            // GetAsCharArray / GetLength for the inline CAnimFilterOfGroup ctor
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#define FABLE_RETAIL_NCREATUREMODE_CMODESCREAMERSUPERATTACK_IMPLICIT_DTOR
#define FABLE_RETAIL_NCREATUREMODE_CMODESCREAMERSUPERATTACK_GETANIMFILTER
#include "fable_string.h"                                   // CCharString (~CCharString 0x0099EAE0)
#include "engine/ETCInterfaceType.h"
#include "engine/NCreatureMode_CModeScreamerSuperAttack.h"

NCreatureMode::CModeScreamerSuperAttack::CModeScreamerSuperAttack(CThing& thing, const CThing& target)
    : CModeStandBase(thing), Filter(CCharString())
{
}
