// CAIStateGroup_SitForMeal::CAIStateGroup_SitForMeal() -- base CAIStateGroup_UsesSeat(), own vtable, then the PPot member (+0x34)
// Leaf: ??0CAIStateGroup_SitForMeal@@QAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FFF74, 36 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128BD34 (RTTI .?AVCAIStateGroup_SitForMeal@@, base CAIStateGroup_UsesSeat : CAIStateGroupBase : CBaseClass); member vtable 0x01238C6C = RTTI .?AV?$CIntelligentPointer@VCThing@@@@; Ego_r.pdb / FableWin.pdb
//   ??0CAIStateGroup_SitForMeal@@QAE@XZ.
// Callees: CAIStateGroup_UsesSeat::CAIStateGroup_UsesSeat 0x008FF1F8 (??0CAIStateGroup_UsesSeat@@IAE@XZ), CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 (??0CBaseIntelligentPointer@@QAE@XZ, Ego_r.pdb / FableWin.pdb).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass, 0x14); Ego_r.pdb CAIStateGroup_SitForMeal::PPot +0x34; the members before it
//   are left unmodelled. CBaseIntelligentPointer / CIntelligentPointer<T>: the chunk 703 model (0x00900B41).
// Shape: user default ctor, empty; CIntelligentPointer's ctor inlines to the base call + vtable store after this class's own vtable store.
// History: generated in a chunk batch for a repeated body shape over the CBase_008ff1f8 / CMemberBase_00a01b10 stand-ins; re-authored on the
//   PDB classes in naming wave 32 (family aisg).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CBaseObjectPointer;
#pragma pack(push, 1)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)
class CThing;

class CAIStateGroup_UsesSeat : public CAIStateGroupBase {                    // RTTI CAIStateGroup_UsesSeat : CAIStateGroupBase
protected:
    CAIStateGroup_UsesSeat();                         // ??0CAIStateGroup_UsesSeat@@IAE@XZ (Ego_r.pdb / FableWin.pdb)
public:
    unsigned char _unmodeled14[0x34 - 0x14];       // Ego_r.pdb: PSeat +0x14, GotSeatPos +0x1c, Sitting +0x1d, SeatPosIndex +0x20, SeatPos +0x24, SittingOnBench +0x30
};

class CAIStateGroup_SitForMeal : public CAIStateGroup_UsesSeat {      // vtable 0x0128BD34
public:
    CIntelligentPointer<CThing> PPot;                     // +0x34
    CAIStateGroup_SitForMeal();
};

CAIStateGroup_SitForMeal::CAIStateGroup_SitForMeal()
{
}
