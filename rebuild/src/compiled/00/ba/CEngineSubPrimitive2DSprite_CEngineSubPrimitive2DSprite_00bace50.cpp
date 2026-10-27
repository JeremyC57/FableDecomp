// CEngineSubPrimitive2DSprite::CEngineSubPrimitive2DSprite(CEngineInternalPrimitive2DSprite* parent) -- the sub-primitive base (parent kept, no next sub-primitive) and this class's vtable
// Leaf: ??0CEngineSubPrimitive2DSprite@@QAE@PAVCEngineInternalPrimitive2DSprite@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00bace50, 34 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): it stores vtable 0x012A5438 (RTTI .?AVCEngineSubPrimitive2DSprite@@ : CEngineSubPrimitiveBase); Ego_r.pdb ??0CEngineSubPrimitive2DSprite@@QAE@PAVCEngineInternalPrimitive2DSprite@@@Z; caller CEnginePrimitiveManager2DSprites::RenderSpritePrim 0x00BB0970 (twice).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0.
// Shape (family L116 = landed family K056, 0x00BB1780): CEngineSubPrimitiveBase's inline constructor (Ego_r.pdb Parent +0x0C,
//   NextSubPrimitive +0x10 cleared); the base's own vptr store is dead and dropped, leaving only 0x012A5438.
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
class CEngineInternalPrimitiveBase { };
class CEngineInternalPrimitive2DSprite : public CEngineInternalPrimitiveBase { };   // RTTI: base at offset 0
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
class CEngineSubPrimitive2DSprite : public CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitive2DSprite(CEngineInternalPrimitive2DSprite* parent);
};
CEngineSubPrimitive2DSprite::CEngineSubPrimitive2DSprite(CEngineInternalPrimitive2DSprite* parent)
    : CEngineSubPrimitiveBase(parent)
{
}