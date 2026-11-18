// CEngineSubPrimitiveBoundingBox::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor chain (CBaseClassNonCopyable::~CBaseClassNonCopyable
//   0x0099A300), then the sub-primitive class operator delete (??3CEngineSubPrimitiveBase@@SIXPAX@Z 0x00BEB550, __fastcall) on flag bit 0
// Leaf: ??_GCEngineSubPrimitiveBoundingBox@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B9C130, 28 bytes, thiscall `ret 4`; slot 0 of the class vtable (rtti-vtables.tsv).
// Names (NF86, round 7, q11117 binder): the class's own scalar deleting destructor (vtable slot 0); the delete is the PDB class operator
//   delete ??3CEngineSubPrimitiveBase@@SIXPAX@Z (FableWin.pdb; the landed self row 0x00BEB550, NF83). Was a free stand-in binding 0x0099A300
//   as ?Destroy@CEngineSubPrimitiveBoundingBox@@QAEXXZ and 0x00BEB550 as ?FreeThis@CEngineSubPrimitiveBoundingBox@@QAEXXZ (self leaf ?VectorDeletingDtor@CEngineSubPrimitiveBoundingBox@@QAEPAXI@Z).
// Shape: VC7.1 emits a class's vtable, and this ??_G with it, in a TU that constructs the class; the header's inline constructor is used
//   by a TU-internal scaffolding function (anonymous namespace, hashed name: not retail; the q11120 pattern), so the vtable, ??_G, implicit
//   destructor and constructor are all pick-any COMDATs here (comdat check). Header: engine/CEngineSubPrimitiveBoundingBox.h with FABLE_RETAIL_PASS22_STATIC_MESH_RENDER (both destructors implicit).
#define FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
#include "fable_compiler.h"
#include "engine/CEngineSubPrimitiveBoundingBox.h"
namespace {
// Fidelity: scaffolding, not retail: the smallest use of the header's inline constructor, so this TU emits CEngineSubPrimitiveBoundingBox's vtable and
// with it the scalar deleting destructor (both pick-any COMDATs). Internal to this TU.
void ReferenceSubPrimitiveBoundingBoxVtable(CEngineInternalPrimitiveOcclusionBase* parent)
{
    CEngineSubPrimitiveBoundingBox sub_primitive(parent);
}
}
