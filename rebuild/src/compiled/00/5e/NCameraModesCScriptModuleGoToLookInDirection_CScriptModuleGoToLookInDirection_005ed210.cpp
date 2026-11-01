// NCameraModes::CScriptModuleGoToLookInDirection::~CScriptModuleGoToLookInDirection() -- compiler-generated destructor: runs the inline base ~CScriptModuleBase (no vtable store of its own)
// Leaf: ??1CScriptModuleGoToLookInDirection@NCameraModes@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005ED210, 34 bytes, thiscall, tail `jmp` to the second member dtor; byte-identical (bar the call
//   displacements) to ~CScriptModuleBase 0x005ED120 and ~CScriptModuleFOVZoom 0x00854380.
// Identity (proven): its only caller is the scalar deleting dtor 0x005ED420 of CScriptModuleTavernGame (vtable 0x01255FC4 slot 0), whose
//   RTTI base-class array (COL of 0x01255FC4) reads TavernGame -> CScriptModuleGoToLookInDirection -> CScriptModuleBase, so the out-of-line
//   callee is the base dtor ~CScriptModuleGoToLookInDirection (TavernGame has no members of its own: Ego_r sizes 100 / 100, and its
//   implicit dtor is inlined into its ??_G). GoToLookInDirection's own ??_G 0x005ED3E0 (vtable 0x01255F9C) expands the same body inline.
//   A compiler-generated destructor stores no vtable of its own (GOTCHAS chunk 694), so the only store is the inlined base dtor's
//   CScriptModuleBase vtable 0x01255F74. Name: Ego_r.pdb ??1CScriptModuleGoToLookInDirection@NCameraModes@@UAE@XZ (the PDB name set
//   holds it beside ??1CScriptModuleTavernGame). The catalog's "CTCIdleScheduler::Clear" is a propagated label.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (PThingOrigin +0x0C, then PCamPoint +0x04).
// Layout: Ego_r.pdb CScriptModuleGoToLookInDirection (100) = CScriptModuleBase (0x4C) + GoTo +0x4C, LookDirection +0x58 (no dtor members).
// Shape: an implicit destructor (no user ~ in the class); the qualified explicit call emits it out of line (the chunk 694 recipe).
class CBaseObject;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();                   // 0x00A01C10
    CBaseObject* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CThing;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class CGameCameraManager;
namespace NCameraModes {
class CBase;
class CScriptModuleBase {
public:
    virtual ~CScriptModuleBase()
    {
        POwner = 0;
        PCameraManager = 0;
    }
    CIntelligentPointer<CThing> PCamPoint;
    CIntelligentPointer<const CThing> PThingOrigin;
    C3DVector CoordOrigin;
    CRightHandedSet CoordAxis;
    CBase* POwner;
    CGameCameraManager* PCameraManager;
    bool SelfTerminate;
    bool ShortenOnCollision;
    long CutOutOverride;
    float FOVOverride;
};
}
namespace NCameraModes {
class CScriptModuleGoToLookInDirection : public CScriptModuleBase {
public:
    CScriptModuleGoToLookInDirection(const C3DVector& go_to, const C3DVector& look_direction);
    C3DVector GoTo;
    C3DVector LookDirection;
};
}
void DestroyModule(NCameraModes::CScriptModuleGoToLookInDirection* p) { p->NCameraModes::CScriptModuleGoToLookInDirection::~CScriptModuleGoToLookInDirection(); }