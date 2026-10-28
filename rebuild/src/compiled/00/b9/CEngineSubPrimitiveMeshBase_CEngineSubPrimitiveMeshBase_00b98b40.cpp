// CEngineSubPrimitiveMeshBase::CEngineSubPrimitiveMeshBase(CEngineInternalPrimitiveBase* parent, C3DPrimitive2* meshPrimitive, ERenderMode renderMode, bool isCloth) -- the sub-primitive base (parent, no next), this class's vtable, the mesh primitive, render mode and cloth flag
// Leaf: ??0CEngineSubPrimitiveMeshBase@@QAE@PAVCEngineInternalPrimitiveBase@@PAVC3DPrimitive2@@W4ERenderMode@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B98B40, 55 bytes, thiscall, `ret 10h`, returns this.
// Identity (proven): stores vtable 0x012A3E8C (RTTI .?AVCEngineSubPrimitiveMeshBase@@ : CEngineSubPrimitiveBase :
//   CBaseClassNonCopyable : CBaseClass); FableWin.pdb
//   ??0CEngineSubPrimitiveMeshBase@@QAE@PAVCEngineInternalPrimitiveBase@@PAVC3DPrimitive2@@W4ERenderMode@@_N@Z (four
//   arguments = `ret 10h`, the bool stored as a byte). The catalog label "CMemoryDataInputStream" is propagated.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0.
// Layout (Ego_r.pdb, retail offsets): CEngineSubPrimitiveBase 0x18 {Parent +0x0C, NextSubPrimitive +0x10}, MeshPrimitive
//   +0x18, RenderMode +0x1C, IsCloth +0x20 (0x24 bytes).
// Shape: CEngineSubPrimitiveBase's inline constructor (as landed 0x00BB1780) then the three member initialisers.
class CBaseClass {
public:
    CBaseClass();                                    // 0x0099A2D0
    virtual ~CBaseClass();                           // 0x0099A2E0
};
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                         // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                         // 0x0099A300 (protected virtual: Ego_r.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};
class CEngineInternalPrimitiveBase;
class CEnginePrimitiveManager;
class C3DPrimitive2;
enum ERenderMode { };

class CEngineSubPrimitiveBase : public CBaseClassNonCopyable {
public:
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase* parent) : Parent(parent), NextSubPrimitive(0) { }
    unsigned long RenderFlags;                       // +0x04
    unsigned char Alpha;                             // +0x08
    CEngineInternalPrimitiveBase* Parent;            // +0x0C
    CEngineSubPrimitiveBase* NextSubPrimitive;       // +0x10
    CEnginePrimitiveManager* Manager;                // +0x14
};

class CEngineSubPrimitiveMeshBase : public CEngineSubPrimitiveBase {
public:
    CEngineSubPrimitiveMeshBase(CEngineInternalPrimitiveBase* parent, C3DPrimitive2* meshPrimitive, ERenderMode renderMode, bool isCloth);
    virtual ~CEngineSubPrimitiveMeshBase();
    C3DPrimitive2* MeshPrimitive;                    // +0x18
    ERenderMode RenderMode;                          // +0x1C
    bool IsCloth;                                    // +0x20
};

CEngineSubPrimitiveMeshBase::CEngineSubPrimitiveMeshBase(CEngineInternalPrimitiveBase* parent, C3DPrimitive2* meshPrimitive, ERenderMode renderMode, bool isCloth)
    : CEngineSubPrimitiveBase(parent),
      MeshPrimitive(meshPrimitive),
      RenderMode(renderMode),
      IsCloth(isCloth)
{
}