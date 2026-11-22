// NCreatureMode::CModeJackOfBladesFloat::~CModeJackOfBladesFloat() -- implicit destructor: PEmitter2, PEmitter, then the Filter (its Group string and the CAnimFilterBase vtable reset), then ~CModeStandBase
// Leaf: ??1CModeJackOfBladesFloat@NCreatureMode@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00740420, 42 bytes, thiscall; tail `jmp ~CModeStandBase` 0x0073D860.
// Identity (proven, two sources): the only caller is the scalar deleting destructor 0x00740400, slot 0 of vtable 0x01267D10
//   (RTTI .?AVCModeJackOfBladesFloat@NCreatureMode@@ : CModeStandBase : CTCCreatureModeManager::CModeBase; the vtable is stored by the
//   constructor at 0x0074035B); FableWin.pdb ??1CModeJackOfBladesFloat@NCreatureMode@@UAE@XZ (0x024BAEF0). It stores no vtable of its own, so it is
//   the compiler-generated destructor. Members (Ego_r.pdb, no STL container, so the offsets are retail's): Filter
//   (CAnimFilterOfGroup) +0x68, PEmitter (CIntelligentPointer<CThing>) +0x74, PEmitter2 (CIntelligentPointer<CThing>) +0x7C; sizeof 0x84.
//   The catalog label "_Dest_val<std::allocator<CMacro>,CMacro>" is propagated and wrong. Byte-near-twins 0x00741390 (~CModeScreamerAggressive) / 0x008AC4D0 (~CModeScreamerSuperAttack) are
//   other classes' implicit destructors (each named from its own ??_G / vtable; three symbols).
// Callees: ??1CBaseIntelligentPointer@@UAE@XZ 0x00A01C10 (twice), ??1CCharString@@QAE@XZ 0x0099EAE0, ??_7CAnimFilterBase@@6B@
//   0x0122FFCC (the inline ~CAnimFilterBase store), ??1CModeStandBase@@UAE@XZ 0x0073D860 (tail).
// Shape: members destroyed in reverse declaration order; the implicit ~CAnimFilterOfGroup and the inline `virtual ~CAnimFilterBase() {}`
//   expand here. Defining retail's own constructor (??0CModeJackOfBladesFloat@NCreatureMode@@QAE@AAVCThing@@@Z; body not reconstructed here, only its
//   member-init order) emits the vtable, hence ??_G and this out-of-line ??1 -- no emission helper.
// Layout (NF247, q12682 header port): CTCCreatureModeManager.h (FABLE_RETAIL_CMODEBASE_VIRTUALS), CModeStandBase.h (FABLE_RETAIL_CMODESTANDBASE
//   + PASS22_VTABLE), CAnimFilterBase.h / CAnimFilterOfGroup.h (INLINE_CTOR: the implicit, inline ~CAnimFilterOfGroup), CIntelligentPointer.h;
//   no header declares NCreatureMode::CModeJackOfBladesFloat, so it stays a row-local class on the shared CModeStandBase base.
// Vtable 0x01267D10 (NF247, q12682): the class's own overrides at slots 4 / 5 / 6 (retail 0x007403F0 / 0x008D9740 / 0x008D7730; Ego_r.pdb /
//   FableWin.pdb / Ego_d.pdb ?GetAnimFilter@CModeJackOfBladesFloat@NCreatureMode@@UBEABVCAnimFilterBase@@XZ, ?OnActivate@..@@UAEXXZ,
//   ?OnDeactivate@..@@UAEXXZ); slot 3 is CModeBase::FrameUpdate 0x0073AC50 (no FrameUpdate override in any PDB).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_PASS22_VTABLE                  // NF242 (q12677): CModeStandBase's slot 2 / 7 / 9 overrides (0x00862EC0 / 0x00861900 / 0x0073DA00)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING                            // GetAsCharArray / GetLength for the inline CAnimFilterOfGroup ctor
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#include "fable_string.h"                                   // CCharString (~CCharString 0x0099EAE0)
#include "engine/ETCInterfaceType.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CModeStandBase.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CIntelligentPointer.h"

class CThing;

namespace NCreatureMode {
class CModeJackOfBladesFloat : public CModeStandBase {                         // vtable 0x01267D10
public:
    CModeJackOfBladesFloat(CThing& thing);
    virtual unsigned long GetType() const;
    virtual const CAnimFilterBase& GetAnimFilter() const;   // slot 4, 0x007403F0
    virtual void OnActivate();                              // slot 5, 0x008D9740
    virtual void OnDeactivate();                            // slot 6, 0x008D7730
    CAnimFilterOfGroup                  Filter;                     // +0x68
    CIntelligentPointer<CThing>        PEmitter;              // +0x74
    CIntelligentPointer<CThing>        PEmitter2;             // +0x7C
};
FABLE_STATIC_ASSERT(sizeof(CModeJackOfBladesFloat) == 0x84);

CModeJackOfBladesFloat::CModeJackOfBladesFloat(CThing& thing)
    : CModeStandBase(thing), Filter(CCharString())
{
}
}
