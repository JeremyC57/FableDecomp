// NShape::CBasicShape::CBasicShape(CTCShapeManager& owner, const std::vector<C3DVector>& pnts) -- CShape base (owner, copied point list, empty selection list) then this class's empty subdivision list
// Flags: /Gr
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00762440, length 61 bytes (+ int3 padding).
// Callees: 0x006F0810 copy constructor of a 12-byte retail vector (the CShape point list at +0x08).
// Vtables: 0x0126A208 NShape::CShape (base, stored first), 0x0126A31C NShape::CBasicShape.
// Shape: the inlined CShape constructor stores its vtable, the id at +0x04, copy-constructs the
// point list at +0x08 out of line and zeroes its own 12-byte vector at +0x14; then the derived
// vtable is stored and the subdivision vector at +0x20 zeroed. The vtable store sits ABOVE the
// zero stores because each zeroed block is a member object with an inline constructor.
// Name fix (NF390 / NF395, T3 nf383 0x009B7870 binder, nf393 re-bin): the self is the PDB constructor ??0CBasicShape@NShape@@QAE@AAVCTCShapeManager@@ABV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z
//   (FableWin.pdb 0x025D8210; the table binds that spelling at 0x00762440 with the compiled witness 007624e0 CTCShapeManager::
//   GetInstantiationFromEnum); was ??0CBasicShape@NShape@@QAE@JABU?$CRetailVector@UC3DVector@NShape@@@@@Z (stand-in CRetailVector,
//   a local NShape::C3DVector and a long id). The class is the shared engine/NShape_CBasicShape.h (CClosedShapeBase : CShape {Owner +4,
//   Points +8, SelectedPoints +0x14}; Subdivisions +0x20), the point list STLport vector<C3DVector> whose copy constructor retail
//   calls out of line at 0x006F0810 (FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY): the call binds
//   ??0?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAE@ABV01@@Z (already bound at 0x006F0810).
// Flags note (NF395): the NShape TU (tc_geometric_shapes) is /Gr: Ego_r.pdb / FableWin.pdb name CShape::StatOnSerialise
//   ?StatOnSerialise@CShape@NShape@@SIX... (a static member that is __fastcall by default), the landed shared-header siblings 007634e0 /
//   00764590 / 00764ee0 compile /Gr, and the STLport helpers this TU instantiates are __fastcall in retail (__destroy_aux<CCountedPointer<
//   CSubdivisionTriangle>*> = 0x00764EE0, landed 00764ee0; the out-of-line ~vector<CCountedPointer<CSubdivisionTriangle>> body matches retail
//   0x00765660 / 0x007657B0 only with its __fastcall __destroy_aux call). The constructor itself (thiscall) is the same either way
//   (build entry: CompilerFlags '/O2 /Oy /W3 /Gr').
#define FABLE_RETAIL_NSHAPE_CONSTRUCTOR_BODIES_PASS26
#define FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY
#include "engine/NShape_CBasicShape.h"

namespace NShape {
CBasicShape::CBasicShape(CTCShapeManager& owner, const std::vector<C3DVector>& pnts)
    : CClosedShapeBase(owner, pnts)
{
}
}
