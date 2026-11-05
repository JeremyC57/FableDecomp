// CAIStateGroup_UsesSeat::CAIStateGroup_UsesSeat() -- base, own vtable, the seat pointer, no seat position yet.
// Leaf: ??0CAIStateGroup_UsesSeat@@IAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Naming wave 32 (family aisg): the ctor is protected (IAE, both PDBs) and the local CAIStateGroupBase (a declared virtual dtor neither PDB has) is engine/CAIStateGroupBase.h's retail branch
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FF1F8, length 51 bytes.
// Callees: CAIStateGroupBase::CAIStateGroupBase() @ 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer()
// @ 0x00A01B10 (PSeat at +0x14, vtable 0x0125A858 = CIntelligentPointer<CThingObject>). Own vtable 0x0128B53C
// (rtti-vtables: CAIStateGroup_UsesSeat; the manifest label "OpinionReactionRoot ctor" is propagated).
// Shape: base constructor, vtable, the member constructor, then SeatPosIndex (+0x20) = -1 (`or ..,-1`) -- an
// init-list member declared after PSeat, so its store follows the member constructor (the vtable store cannot sink
// below a member constructor call) -- then the body: GotSeatPos (+0x1c), Sitting (+0x1d) and SittingOnBench
// (+0x30) cleared from one `xor al,al` (as body assignments VC schedules the -1 store after them). Member names and
// offsets from struct_layouts_egor.tsv (CAIStateGroup_UsesSeat, 52 bytes); SeatPos (+0x24) is left alone.
// Pragma: `#pragma optimize("s", on)` (the `or [mem],-1` store is the size-optimised -1 initialiser).
#pragma optimize("s", on)
class CThing;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CThingObject;

#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CAIStateGroup_UsesSeat : public CAIStateGroupBase {   // vtable 0x0128B53C
public:
    CIntelligentPointer<CThingObject> PSeat;            // +0x14
    bool GotSeatPos;                            // +0x1c
    bool Sitting;                               // +0x1d
    long SeatPosIndex;                          // +0x20
    float SeatPos_X, SeatPos_Y, SeatPos_Z;      // +0x24 C3DVector
    bool SittingOnBench;                        // +0x30
protected:
    CAIStateGroup_UsesSeat();                   // ??0CAIStateGroup_UsesSeat@@IAE@XZ (Ego_r.pdb / FableWin.pdb: protected)
};

CAIStateGroup_UsesSeat::CAIStateGroup_UsesSeat()
    : CAIStateGroupBase()
    , PSeat()
    , SeatPosIndex(-1)
{
    GotSeatPos = false;
    Sitting = false;
    SittingOnBench = false;
}