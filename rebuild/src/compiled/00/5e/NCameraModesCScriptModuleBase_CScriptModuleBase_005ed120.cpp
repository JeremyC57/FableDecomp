// NCameraModes::CScriptModuleBase::~CScriptModuleBase() -- clear the owner and camera-manager pointers, destroy PThingOrigin and PCamPoint
// Leaf: ??1CScriptModuleBase@NCameraModes@@UAE@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x005ED120, 34 bytes, thiscall, tail `jmp` to the second member dtor.
// Identity (proven): the body stores the CScriptModuleBase vtable 0x01255F74 (RTTI .?AVCScriptModuleBase@NCameraModes@@, one base);
//   its callers are the ??_G of classes deriving DIRECTLY from CScriptModuleBase (CScriptModuleTransitionBetweenPoints 0x00854450,
//   CScriptModuleFullTransition 0x008545B0, CScriptModuleSpline 0x008545D0, CScriptModuleCameraOnAStick 0x007AB4A0 slot 7 area ...;
//   RTTI base-class arrays read from the COL), so it is ??1CScriptModuleBase@NCameraModes@@UAE@XZ (Ego_r.pdb + FableWin.pdb name
//   sets). The byte-identical 0x00854310 is the SelfTerminateBase dtor (see that row); the catalog's "CTCIdleScheduler::Clear"
//   is a propagated label.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (PThingOrigin +0x0C, then PCamPoint +0x04).
// Layout: Ego_r.pdb CScriptModuleBase (0x4C): PCamPoint +0x04, PThingOrigin +0x0C, CoordOrigin, CoordAxis, POwner +0x38,
//   PCameraManager +0x3C, SelfTerminate, ShortenOnCollision, CutOutOverride, FOVOverride (no engine header exists).
// Shape: the user inline virtual dtor of the landed ??_G 0x005ED150 model; the qualified explicit call emits the out-of-line COMDAT.
class CBaseObject;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();
    virtual ~CBaseIntelligentPointer();                   // 0x00A01C10
    CBaseObject* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
class CThing;
#include "engine/C3DVector.h"
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
void DestroyModule(NCameraModes::CScriptModuleBase* p) { p->NCameraModes::CScriptModuleBase::~CScriptModuleBase(); }