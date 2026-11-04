// CAIStateGroup_KidPlayHopscotch::CAIStateGroup_KidPlayHopscotch() -- base CAIStateGroupBase(), own vtable, then the PHopscotchBoard member (CIntelligentPointer<CThing>, +0x15)
// Leaf: ??0CAIStateGroup_KidPlayHopscotch@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FFBEE, 36 bytes, thiscall, `ret'. A real function in the AI state-group TU, which
//   retail compiled without function alignment (odd start addresses, `pop ecx' cleanups throughout 0x008FD000..0x00903xxx); the
//   catalog's "CAIStateGroupBehaviourBase::CAIStateGroupBehaviourBase" is propagated across the byte-identical ctor bodies.
// Identity: stores own vtable 0x0128BB90 (RTTI .?AVCAIStateGroup_KidPlayHopscotch@@, rtti-vtables.tsv; its slot 0 is the landed scalar deleting
//   destructor right after this body); member vtable 0x01238C6C = RTTI .?AV?$CIntelligentPointer@VCThing@@@@; Ego_r.pdb / FableWin.pdb
//   ??0CAIStateGroup_KidPlayHopscotch@@QAE@XZ. Sole caller: the factory thunk 0x00902448 (`operator new(0x68)` then `jmp` here; 0x68 = the Ego_r.pdb class size).
// Callees: CAIStateGroupBase::CAIStateGroupBase 0x008FD255 (??0CAIStateGroupBase@@QAE@XZ), CBaseIntelligentPointer::CBaseIntelligentPointer
//   0x00A01B10 (??0CBaseIntelligentPointer@@QAE@XZ, Ego_r.pdb / FableWin.pdb).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass, 0x14); Ego_r.pdb CAIStateGroup_KidPlayHopscotch: TookActionPoint bool
//   +0x14, PHopscotchBoard +0x15 (the retail `lea edi,[esi+15h]'): engine/CIntelligentPointer.h's retail branch packs the pointer classes
//   (alignment 1), so the member follows the bool directly; the members after it are not touched by the ctor and are left unmodelled.
// Shape: user default ctor, empty (the landed 0x008FFBA4 KidWatchHopscotch model); CIntelligentPointer's inline ctor expands to the base
//   call and its vtable store after this class's own vtable store.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"                // CBaseIntelligentPointer (packed), ctor 0x00A01B10; CIntelligentPointer<T>
class CThing;

class CAIStateGroup_KidPlayHopscotch : public CAIStateGroupBase {      // vtable 0x0128BB90
public:
    bool TookActionPoint;                                              // +0x14
    CIntelligentPointer<CThing> PHopscotchBoard;                                // +0x15
    CAIStateGroup_KidPlayHopscotch();
};

CAIStateGroup_KidPlayHopscotch::CAIStateGroup_KidPlayHopscotch()
{
}