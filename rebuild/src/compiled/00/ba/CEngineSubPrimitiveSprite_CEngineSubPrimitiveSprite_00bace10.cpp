// CEngineSubPrimitiveSprite::CEngineSubPrimitiveSprite(CEngineInternalPrimitiveSprite* parent) -- the sub-primitive base (parent kept, no next sub-primitive) and this class's vtable
// Leaf: ??0CEngineSubPrimitiveSprite@@QAE@PAVCEngineInternalPrimitiveSprite@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00bace10, 34 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): it stores vtable 0x012A5430 (RTTI .?AVCEngineSubPrimitiveSprite@@ : CEngineSubPrimitiveBase); FableWin.pdb ??0CEngineSubPrimitiveSprite@@QAE@PAVCEngineInternalPrimitiveSprite@@@Z; no direct callers found (unbound).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0.
// Shape (family L116 = landed family K056, 0x00BB1780): CEngineSubPrimitiveBase's inline constructor (Ego_r.pdb Parent +0x0C,
//   NextSubPrimitive +0x10 cleared); the base's own vptr store is dead and dropped, leaving only 0x012A5430.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                             // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};
class CEngineInternalPrimitiveBase { };
class CEngineInternalPrimitiveSprite : public CEngineInternalPrimitiveBase { };   // RTTI: base at offset 0
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
class CEngineSubPrimitiveSprite : public CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitiveSprite(CEngineInternalPrimitiveSprite* parent);
};
CEngineSubPrimitiveSprite::CEngineSubPrimitiveSprite(CEngineInternalPrimitiveSprite* parent)
    : CEngineSubPrimitiveBase(parent)
{
}