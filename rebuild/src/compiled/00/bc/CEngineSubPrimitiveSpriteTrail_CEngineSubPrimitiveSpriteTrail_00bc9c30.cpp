// CEngineSubPrimitiveSpriteTrail::CEngineSubPrimitiveSpriteTrail(CEngineInternalPrimitiveSpriteTrail* parent) -- the sub-primitive base (parent kept, no next sub-primitive) and this class's vtable
// Leaf: ??0CEngineSubPrimitiveSpriteTrail@@QAE@PAVCEngineInternalPrimitiveSpriteTrail@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC9C30, 34 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): stores vtable 0x012A69D4 (RTTI .?AVCEngineSubPrimitiveSpriteTrail@@, slot 0 = ??_G 0x00BCA060);
//   Ego_r.pdb ??0CEngineSubPrimitiveSpriteTrail@@QAE@PAVCEngineInternalPrimitiveSpriteTrail@@@Z (parameter type from the PDB,
//   not CEngineInternalPrimitiveBase as the earlier landed source spelled it); no direct callers (reached from the
//   CEngineInternalPrimitiveSpriteTrail side through an inlined new).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0 (protected, engine/CBaseClassNonCopyable.h).
// Shape (same as the landed siblings 0x00BBA260 CEngineSubPrimitiveDecal / 0x00BB6950 WeatherMask): CEngineSubPrimitiveBase's
//   inline constructor (Ego_r.pdb ??0CEngineSubPrimitiveBase@@QAE@PAVCEngineInternalPrimitiveBase@@@Z; members Parent +0x0C,
//   NextSubPrimitive +0x10 cleared; RenderFlags / Alpha / Manager left for the renderer); the base's own vptr store is dead and
//   dropped, leaving only the derived one. Both destructors are compiler-generated (the ??_G at slot 0 calls
//   ~CBaseClassNonCopyable 0x0099A300 directly).
#include "engine/CBaseClassNonCopyable.h"
// RTTI: CEngineInternalPrimitiveSpriteTrail : CEngineInternalPrimitiveBase at offset 0 (members not touched here;
// engine/CEngineInternalPrimitiveBase.h includes <stddef.h>, which hangs cl in this lane -- header debt).
class CEngineInternalPrimitiveBase { };
class CEngineInternalPrimitiveSpriteTrail : public CEngineInternalPrimitiveBase { };
class CEnginePrimitiveManager;
class CEngineSubPrimitiveBase : public CBaseClassNonCopyable {   // Ego_r.pdb, 0x18 bytes
public:
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase* parent) : Parent(parent), NextSubPrimitive(0) { }
    unsigned long RenderFlags;                           // +0x04
    unsigned char Alpha;                                 // +0x08
    CEngineInternalPrimitiveBase* Parent;                // +0x0C
    CEngineSubPrimitiveBase* NextSubPrimitive;           // +0x10
    CEnginePrimitiveManager* Manager;                    // +0x14
};
class CEngineSubPrimitiveSpriteTrail : public CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitiveSpriteTrail(CEngineInternalPrimitiveSpriteTrail* parent);
};
CEngineSubPrimitiveSpriteTrail::CEngineSubPrimitiveSpriteTrail(CEngineInternalPrimitiveSpriteTrail* parent)
    : CEngineSubPrimitiveBase(parent)
{
}