// NCameraModes::CScriptModuleFOVZoom::~CScriptModuleFOVZoom() -- compiler-generated destructor: runs the inline base ~CScriptModuleBase (no vtable store of its own)
// Leaf: ??1CScriptModuleFOVZoom@NCameraModes@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00854380, 34 bytes, thiscall, tail `jmp` to the second member dtor; byte-identical (bar the call
//   displacements) to ~CScriptModuleBase 0x005ED120 and ~CScriptModuleGoToLookInDirection 0x005ED210.
// Identity (proven): its only caller is the scalar deleting dtor 0x008544C0 of CScriptModuleFOVZoomStayRelative (vtable 0x01275A94 slot 0),
//   which first destroys its own intelligent-pointer member at +0x84 (FOVZoomStayRelative's implicit dtor, inlined) and then calls this
//   body; the RTTI base-class array of 0x01275A94 reads FOVZoomStayRelative -> CScriptModuleFOVZoom -> CScriptModuleBase, so this is the
//   base dtor ~CScriptModuleFOVZoom. FOVZoom's own ??_G 0x00854480 (vtable 0x01275A1C) expands the same body inline. A compiler-generated
//   destructor stores no vtable of its own (GOTCHAS chunk 694), so the only store is the inlined base dtor's CScriptModuleBase vtable
//   0x01255F74. Name: Ego_r.pdb ??1CScriptModuleFOVZoom@NCameraModes@@UAE@XZ (beside ??1CScriptModuleFOVZoomStayRelative). The catalog's
//   "CTCIdleScheduler::Clear" is a propagated label.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (PThingOrigin +0x0C, then PCamPoint +0x04).
// Layout: Ego_r.pdb CScriptModuleFOVZoom (132) = CScriptModuleBase (0x4C) + LookAtPos, EndFrame, WorldFrames, FOVIncrement, FOVFraction,
//   Increment, StartPos, ShortenOnOcclusion, ResetCamera (no dtor members); the FOVZoomStayRelative member at +0x84 (= 132) confirms the size.
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
class CScriptModuleFOVZoom : public CScriptModuleBase {
public:
    CScriptModuleFOVZoom(const C3DVector& look_at_pos, const C3DVector& increment, const C3DVector& start_pos, float fov_increment, long world_frames, bool reset_camera);
    C3DVector LookAtPos;
    long EndFrame;
    long WorldFrames;
    float FOVIncrement;
    float FOVFraction;
    C3DVector Increment;
    C3DVector StartPos;
    bool ShortenOnOcclusion;
    bool ResetCamera;
};
}
void DestroyModule(NCameraModes::CScriptModuleFOVZoom* p) { p->NCameraModes::CScriptModuleFOVZoom::~CScriptModuleFOVZoom(); }