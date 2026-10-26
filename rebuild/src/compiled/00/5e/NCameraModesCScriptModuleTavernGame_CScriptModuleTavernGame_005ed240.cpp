// NCameraModes::CScriptModuleTavernGame::CScriptModuleTavernGame(const C3DVector& goTo, const C3DVector& lookDirection) -- the base constructor with both arguments, then this class's vtable
// Leaf: ??0CScriptModuleTavernGame@NCameraModes@@QAE@ABVC3DVector@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005ED240, 32 bytes (+ int3 padding), thiscall, `ret 8`. Vtable 0x01255FC4.
// Identity (proven): NCameraModes::CScriptModuleTavernGame::CScriptModuleTavernGame(const C3DVector& goTo, const C3DVector& lookDirection); evidence: the stored vtable 0x01255FC4's RTTI is NCameraModes::CScriptModuleTavernGame (<- CScriptModuleGoToLookInDirection <- CScriptModuleBase), the base call 0x005ED190 stores CScriptModuleGoToLookInDirection's vtable 0x01255F9C; its one caller is CTCTavernGame::CameraUseCameraPoint 0x005EBFA0 (new + construct); FableWin.pdb ??0CScriptModuleTavernGame@NCameraModes@@QAE@ABVC3DVector@@0@Z.
// Label: the manifest's "CDrawAutoPickups" is propagated (family J100: five byte-identical two-argument forwarding constructors;
//   retail has no identical-code folding).
// Callees: NCameraModes::CScriptModuleGoToLookInDirection::CScriptModuleGoToLookInDirection(const C3DVector&, const C3DVector&) 0x005ED190.
// Layout: RTTI NCameraModes::CScriptModuleTavernGame <- CScriptModuleGoToLookInDirection <- CScriptModuleBase; the base stores GoTo (+0x4C) and LookDirection (+0x58). Local polymorphic partial views (the engine headers of this chain, where they exist, are Ego_r layouts
//   with the vptr as a data word); the class adds no members.
class C3DVector {                                         // 12 bytes (Ego_r.pdb)
public:
    float X;                                               // +0x00
    float Y;                                               // +0x04
    float Z;                                               // +0x08
};
namespace NCameraModes {
class CScriptModuleBase {                                 // partial view: polymorphic root (RTTI NCameraModes::CScriptModuleBase)
public:
    virtual ~CScriptModuleBase();
};
class CScriptModuleGoToLookInDirection : public CScriptModuleBase {   // partial view (Ego_r.pdb GoTo +0x4C, LookDirection +0x58)
public:
    CScriptModuleGoToLookInDirection(const C3DVector& goTo, const C3DVector& lookDirection);   // 0x005ED190
    virtual ~CScriptModuleGoToLookInDirection();
};
class CScriptModuleTavernGame : public CScriptModuleGoToLookInDirection {
public:
    CScriptModuleTavernGame(const C3DVector& goTo, const C3DVector& lookDirection);
    virtual ~CScriptModuleTavernGame();
};

CScriptModuleTavernGame::CScriptModuleTavernGame(const C3DVector& goTo, const C3DVector& lookDirection)
    : CScriptModuleGoToLookInDirection(goTo, lookDirection)
{
}
}   // namespace NCameraModes