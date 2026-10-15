// NCameraModes::CScriptModuleBase::CScriptModuleBase() -- constructor: two empty intelligent pointers (PCamPoint, PThingOrigin), zero origin, the default axis set (Up = Z, Forward = X), no owner/manager, flags clear, no cut-out or FOV override (-1)
// Leaf: ??0CScriptModuleBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005ED070, length 112 bytes (107 + int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 (twice).
// Vtables: 0x01255F74 (NCameraModes::CScriptModuleBase), 0x01238C6C (CIntelligentPointer<CThing>),
// 0x01237F1C (CIntelligentPointer<const CThing>).
// Shape: initialiser list in declaration order; each intelligent pointer is its out-of-line base ctor
// plus the inline derived vtable store; the C3DVector members use an inline (x,y,z) constructor and
// share one zero register and the 1.0f constant in ecx.
// Layout: PDB NCameraModes::CScriptModuleBase (0x4c bytes): PCamPoint +4, PThingOrigin +0xc,
// CoordOrigin +0x14, CoordAxis +0x20 (Up, Forward), POwner +0x38, PCameraManager +0x3c,
// SelfTerminate +0x40, ShortenOnCollision +0x41, CutOutOverride +0x44, FOVOverride +0x48.
struct CBaseObject;
struct CThing;
struct CBase;
struct CGameCameraManager;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                       // 0x00A01B10
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {        // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer_CThing() {}
};
struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer_CThingConst() {}
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet() : Up(0.0f, 0.0f, 1.0f), Forward(1.0f, 0.0f, 0.0f) {}
};

namespace NCameraModes {

struct CScriptModuleBase {                           // vtable 0x01255F74
    virtual void v00();
    CIntelligentPointer_CThing      PCamPoint;       // +0x04
    CIntelligentPointer_CThingConst PThingOrigin;    // +0x0c
    C3DVector                       CoordOrigin;     // +0x14
    CRightHandedSet                 CoordAxis;       // +0x20
    CBase*                          POwner;          // +0x38
    CGameCameraManager*             PCameraManager;  // +0x3c
    bool                            SelfTerminate;   // +0x40
    bool                            ShortenOnCollision; // +0x41
    long                            CutOutOverride;  // +0x44
    float                           FOVOverride;     // +0x48
    CScriptModuleBase();
};

CScriptModuleBase::CScriptModuleBase()
    : PCamPoint(), PThingOrigin(), CoordOrigin(0.0f, 0.0f, 0.0f), CoordAxis(),
      POwner(0), PCameraManager(0), SelfTerminate(false), ShortenOnCollision(false),
      CutOutOverride(-1), FOVOverride(-1.0f)
{
}

}