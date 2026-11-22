// N3DPrimitive::CAnimatedBlock::CDeviceIndependent::~CDeviceIndependent() -- the implicit virtual destructor: ~CArray<C3DIndexedBoneWeight> (free the buffer at +0x14 when held), ~CArray<unsigned long> (+0x08), then the CIVCountedPointeeBase base (its vtable 0x012354D0 stored last)
// Leaf: ??1CDeviceIndependent@CAnimatedBlock@N3DPrimitive@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A978C0, 43 bytes. Callees: free 0x00BFEA14 (twice). Caller: the scalar deleting destructor 0x00A978A0 (its only one).
// Model: engine/C3DPrimitive2.h FABLE_RETAIL_CANIMATEDBLOCK_DATA: CAnimatedBlock::CDeviceIndependent : CIVCountedPointeeBase (vtable 0x012354D0) with
//   CArray<unsigned long> VertexList +0x08 and CArray<C3DIndexedBoneWeight> BoneWeights +0x14 (Ego_r.pdb), the implicit virtual destructor.
//   Vtable 0x0129DF88 (RTTI .?AVCDeviceIndependent@CAnimatedBlock@N3DPrimitive@@, rtti-vtables.tsv): slot 0 = 0x00A978A0, the scalar deleting
//   destructor, which calls the destructor 0x00A978C0 out of line; slot 1 = CIVCountedPointeeBase::Delete.
// Names: FableWin.pdb ??1CDeviceIndependent@CAnimatedBlock@N3DPrimitive@@UAE@XZ / ??_ECDeviceIndependent@...@@UAEPAXI@Z (debug builds emit
//   ??_E; VC7.1 /O2 emits ??_G and the vtable names the weak alias ??_E -> ??_G) + the retail body and vtable slot (second source).
// Prelude: the #define / #include block of the landed same-TU 0x00A90AE0 CAnimatedBlock::InitialiseFromMeshFileData, whose HEAD object already
//   defines this class's vtable, ??_G and ??1 from the same headers (both COMDATs EQ to retail 0x00A978A0 / 0x00A978C0: pk.py).
// Names (agentNF254, 2026-11-22, CODEXP27 L114): the two rows were the stand-ins ??1CCountedBufferPair_00a978c0@@QAE@XZ (a novtable row-local
//   model) and the free __fastcall ?CDeviceIndependent_scalar_deleting_destructor@@YIPAXPAUCDeviceIndependent@@HI@Z binding
//   ?CDeviceIndependent_dtor@@YIXPAUCDeviceIndependent@@@Z; both now emit the members the compiler generates for the shared header class.
#define FABLE_RETAIL_ANIMATEDPRIMITIVEBLOCK_ACCESSORS_PASS27
#define FABLE_RETAIL_C3DPRIMITIVE2_LOADER
#define FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
#define FABLE_RETAIL_CANIMATEDBLOCK_DATA
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ORIGINAL_CASTTOBASE_PASS22
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DINDEXEDBONEWEIGHT_CTOR_PASS27
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_STLP_VECTOR_ULONG_ERASE_COPY_PASS27
#include <string.h>
#include "fable_assert.h"
#include "engine/EVertexType.h"
#include "engine/C3DPrimitive2.h"

namespace {
// Fidelity: scaffolding, not retail: constructing one makes VC7.1 emit this class's vftable with its scalar deleting destructor and the
// implicit destructor (this row's leaf), as the same-TU 0x00A90AE0 does with Data.Reset(new CDeviceIndependent). Internal to this TU
// (anonymous namespace, so no two rows define the same harness symbol); never called.
N3DPrimitive::CAnimatedBlock::CDeviceIndependent* EmitDeviceIndependentVirtuals()
{
    return new N3DPrimitive::CAnimatedBlock::CDeviceIndependent;
}
}
