// CAIStateGroup_WorkHammerAnvil::CAIStateGroup_WorkHammerAnvil() -- base CAIStateGroupBase(), own vtable, then the PAnvil member (CIntelligentPointer<CThingObject>, +0x15)
// Leaf: ??0CAIStateGroup_WorkHammerAnvil@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FFDCE, 36 bytes, thiscall, `ret'. A real function in the AI state-group TU, which
//   retail compiled without function alignment (odd start addresses, `pop ecx' cleanups throughout 0x008FD000..0x00903xxx); the
//   catalog's "CAIStateGroupBehaviourBase::CAIStateGroupBehaviourBase" is propagated across the byte-identical ctor bodies.
// Identity: stores own vtable 0x0128BCF8 (RTTI .?AVCAIStateGroup_WorkHammerAnvil@@, rtti-vtables.tsv; its slot 0 is the landed scalar deleting
//   destructor right after this body); member vtable 0x0125A858 = RTTI .?AV?$CIntelligentPointer@VCThingObject@@@@; Ego_r.pdb / FableWin.pdb
//   ??0CAIStateGroup_WorkHammerAnvil@@QAE@XZ. Sole caller: the factory thunk 0x009024E8 (`operator new(0x4C)` then `jmp` here; 0x4C = the Ego_r.pdb class size).
// Callees: CAIStateGroupBase::CAIStateGroupBase 0x008FD255 (??0CAIStateGroupBase@@QAE@XZ), CBaseIntelligentPointer::CBaseIntelligentPointer
//   0x00A01B10 (??0CBaseIntelligentPointer@@QAE@XZ, Ego_r.pdb / FableWin.pdb).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass, 0x14); Ego_r.pdb CAIStateGroup_WorkHammerAnvil: TookActionPoint bool
//   +0x14, PAnvil +0x15 (the retail `lea edi,[esi+15h]'): engine/CIntelligentPointer.h's retail branch packs the pointer classes
//   (alignment 1), so the member follows the bool directly; the members after it are not touched by the ctor and are left unmodelled.
// Shape: user default ctor, empty (the landed 0x008FFBA4 KidWatchHopscotch model); CIntelligentPointer's inline ctor expands to the base
//   call and its vtable store after this class's own vtable store.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"                // CBaseIntelligentPointer (packed), ctor 0x00A01B10; CIntelligentPointer<T>
class CThingObject;

class CAIStateGroup_WorkHammerAnvil : public CAIStateGroupBase {      // vtable 0x0128BCF8
public:
    bool TookActionPoint;                                              // +0x14
    CIntelligentPointer<CThingObject> PAnvil;                                // +0x15
    CAIStateGroup_WorkHammerAnvil();
};

CAIStateGroup_WorkHammerAnvil::CAIStateGroup_WorkHammerAnvil()
{
}