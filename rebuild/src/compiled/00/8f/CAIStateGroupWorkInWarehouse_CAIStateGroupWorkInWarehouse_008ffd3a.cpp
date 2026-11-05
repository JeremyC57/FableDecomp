// CAIStateGroup_WorkInWarehouse::CAIStateGroup_WorkInWarehouse() -- base CAIStateGroup_MoveGoodsBase(), own vtable, then the PTask member (+0x1c)
// Leaf: ??0CAIStateGroup_WorkInWarehouse@@QAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FFD3A, 36 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128BC80 (RTTI .?AVCAIStateGroup_WorkInWarehouse@@, base CAIStateGroup_MoveGoodsBase : CAIStateGroup_WorkBase<CAIStateGroupBase> : CAIStateGroupBase : CBaseClass); member vtable 0x0128B0B0 = RTTI .?AV?$CIntelligentPointer@VCVillageTaskCarryCrate@@@@; Ego_r.pdb / FableWin.pdb
//   ??0CAIStateGroup_WorkInWarehouse@@QAE@XZ.
// Callees: CAIStateGroup_MoveGoodsBase::CAIStateGroup_MoveGoodsBase 0x008FDA5D (??0CAIStateGroup_MoveGoodsBase@@QAE@XZ), CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 (??0CBaseIntelligentPointer@@QAE@XZ, Ego_r.pdb / FableWin.pdb).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass, 0x14); Ego_r.pdb CAIStateGroup_WorkInWarehouse::PTask +0x1c; the members before it
//   are left unmodelled. CBaseIntelligentPointer / CIntelligentPointer<T>: the chunk 703 model (0x00900B41).
// Shape: user default ctor, empty; CIntelligentPointer's ctor inlines to the base call + vtable store after this class's own vtable store.
// History: generated in a chunk batch for a repeated body shape over the CBase_008fda5d / CMemberBase_00a01b10 stand-ins; re-authored on the
//   PDB classes in naming wave 32 (family aisg).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CBaseObjectPointer;
#pragma pack(push, 1)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)
class CVillageTaskCarryCrate;

template <class T> class CAIStateGroup_WorkBase : public T { };              // RTTI CAIStateGroup_WorkBase<CAIStateGroupBase> (no ctor of its own)

class CAIStateGroup_MoveGoodsBase : public CAIStateGroup_WorkBase<CAIStateGroupBase> {                    // RTTI CAIStateGroup_MoveGoodsBase : CAIStateGroup_WorkBase<CAIStateGroupBase>
public:
    CAIStateGroup_MoveGoodsBase();                         // ??0CAIStateGroup_MoveGoodsBase@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
    unsigned char _unmodeled14[0x1c - 0x14];       // Ego_r.pdb: DroppingGoods +0x14, PickUpGoodsActionID +0x18
};

class CAIStateGroup_WorkInWarehouse : public CAIStateGroup_MoveGoodsBase {      // vtable 0x0128BC80
public:
    CIntelligentPointer<CVillageTaskCarryCrate> PTask;                     // +0x1c
    CAIStateGroup_WorkInWarehouse();
};

CAIStateGroup_WorkInWarehouse::CAIStateGroup_WorkInWarehouse()
{
}
