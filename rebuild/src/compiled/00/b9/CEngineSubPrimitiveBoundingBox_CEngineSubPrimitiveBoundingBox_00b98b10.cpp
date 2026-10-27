// CEngineSubPrimitiveBoundingBox::CEngineSubPrimitiveBoundingBox(CEngineInternalPrimitiveOcclusionBase* parent) -- the sub-primitive base (parent kept, no next sub-primitive) and this class's vtable
// Leaf: ??0CEngineSubPrimitiveBoundingBox@@QAE@PAVCEngineInternalPrimitiveOcclusionBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B98B10, 34 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): stores vtable 0x012A3E84 (RTTI .?AVCEngineSubPrimitiveBoundingBox@@ : CEngineSubPrimitiveBase : CBaseClassNonCopyable : CBaseClass);
//   FableWin.pdb ??0CEngineSubPrimitiveBoundingBox@@QAE@PAVCEngineInternalPrimitiveOcclusionBase@@@Z; callers the RenderPrimitive members 0x00B9A4A0 / 0x00BA115C / 0x00BABF70 (frame-local sub-primitives).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0.
// Shape (family K056, representative 0x00B98B10): CEngineSubPrimitiveBase's inline constructor (Ego_r.pdb members: Parent +0x0C,
//   NextSubPrimitive +0x10 cleared; RenderFlags / Alpha / Manager left for the renderer); the base's own vptr store is dead
//   and dropped, leaving only the derived one.
class CBaseClass {
public:
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();
};
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                             // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};
// RTTI: CEngineInternalPrimitiveOcclusionBase : CEngineInternalPrimitiveBase at offset 0 (members not touched here)
class CEngineInternalPrimitiveBase { };
class CEngineInternalPrimitiveOcclusionBase : public CEngineInternalPrimitiveBase { };
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
class CEngineSubPrimitiveBoundingBox : public CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitiveBoundingBox(CEngineInternalPrimitiveOcclusionBase* parent);
};
CEngineSubPrimitiveBoundingBox::CEngineSubPrimitiveBoundingBox(CEngineInternalPrimitiveOcclusionBase* parent)
    : CEngineSubPrimitiveBase(parent)
{
}