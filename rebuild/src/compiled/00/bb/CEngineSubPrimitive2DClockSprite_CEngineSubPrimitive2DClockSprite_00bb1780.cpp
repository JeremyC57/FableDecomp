// CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite(CEngineInternalPrimitive2DClockSprite* parent) -- the sub-primitive base (parent kept, no next sub-primitive) and this class's vtable
// Leaf: ??0CEngineSubPrimitive2DClockSprite@@QAE@PAVCEngineInternalPrimitive2DClockSprite@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BB1780, 34 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): stores vtable 0x012A55AC (RTTI .?AVCEngineSubPrimitive2DClockSprite@@ : CEngineSubPrimitiveBase : CBaseClassNonCopyable : CBaseClass);
//   FableWin.pdb ??0CEngineSubPrimitive2DClockSprite@@QAE@PAVCEngineInternalPrimitive2DClockSprite@@@Z; caller CEnginePrimitiveManager2DSprites::RenderClockPrim 0x00BAFA00.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0.
// Shape (family K056, representative 0x00B98B10): CEngineSubPrimitiveBase's inline constructor (Ego_r.pdb members: Parent +0x0C,
//   NextSubPrimitive +0x10 cleared; RenderFlags / Alpha / Manager left for the renderer); the base's own vptr store is dead
//   and dropped, leaving only the derived one.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                             // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};
// RTTI: CEngineInternalPrimitive2DClockSprite : CEngineInternalPrimitiveBase at offset 0 (members not touched here)
class CEngineInternalPrimitiveBase { };
class CEngineInternalPrimitive2DClockSprite : public CEngineInternalPrimitiveBase { };
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
class CEngineSubPrimitive2DClockSprite : public CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitive2DClockSprite(CEngineInternalPrimitive2DClockSprite* parent);
};
CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite(CEngineInternalPrimitive2DClockSprite* parent)
    : CEngineSubPrimitiveBase(parent)
{
}