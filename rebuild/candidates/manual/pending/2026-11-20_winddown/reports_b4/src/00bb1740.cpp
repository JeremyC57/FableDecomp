// CEngineSubPrimitiveSprite::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor chain (CBaseClassNonCopyable::~CBaseClassNonCopyable
//   0x0099A300), then the sub-primitive class operator delete (??3CEngineSubPrimitiveBase@@SIXPAX@Z 0x00BEB550, __fastcall) on flag bit 0
// Leaf: ??_GCEngineSubPrimitiveSprite@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BB1740, 28 bytes, thiscall `ret 4`; slot 0 of the class vtable 0x012A5430 (RTTI
//   .?AVCEngineSubPrimitiveSprite@@ : CEngineSubPrimitiveBase; the vtable the constructor 0x00BACE10 stores).
// Re-port (Account B, 2026-11-20): onto the shared engine/CEngineSubPrimitiveSprite.h, as the sibling 0x00BCA060
//   CEngineSubPrimitiveSpriteTrail::`scalar deleting destructor' (NF86). Was a free stand-in (`struct CEngineSubPrimitiveSprite` with
//   ?Destroy@ binding 0x0099A300, ?VDD@ as the self leaf and a free `FreeMesh` binding 0x00BEB550).
// Shape: VC7.1 emits a class's vtable, and this ??_G with it, in a TU that constructs the class; with the headers' implicit destructors
//   (FABLE_RETAIL_PASS22_PRIMITIVE_IMPLICIT_DESTRUCTORS) the derived and base destructors are empty and inline, leaving the
//   ~CBaseClassNonCopyable call, and the delete is the class operator delete. The header's inline constructor is used by a TU-internal
//   scaffolding function (anonymous namespace, hashed name: not retail; the q11120 / 0x00BCA060 pattern), so the vtable, ??_G, implicit
//   destructor and constructor are pick-any COMDATs here.
#define FABLE_RETAIL_PASS22_PRIMITIVE_IMPLICIT_DESTRUCTORS
#include "engine/CEngineSubPrimitiveSprite.h"
namespace {
// Fidelity: scaffolding, not retail: the smallest use of the header's inline constructor, so this TU emits CEngineSubPrimitiveSprite's vtable and
// with it the scalar deleting destructor (both pick-any COMDATs). Internal to this TU.
void ReferenceSubPrimitiveSpriteVtable(CEngineInternalPrimitiveSprite* parent)
{
    CEngineSubPrimitiveSprite sub_primitive(parent);
}
}
