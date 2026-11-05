// NCameraModes::CScriptModuleSelfTerminateBase::~CScriptModuleSelfTerminateBase() -- compiler-generated destructor: runs the inline base ~CScriptModuleBase (no vtable store of its own)
// Leaf: ??1CScriptModuleSelfTerminateBase@NCameraModes@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00854310, 34 bytes, thiscall; byte-identical (bar the call displacements) to ~CScriptModuleBase 0x005ED120.
// Identity (proven): its only callers are the scalar deleting dtors of CScriptModuleOpenChest 0x00854500, CScriptModuleUseLamp
//   0x00854540 and CScriptModuleOpenGate 0x00854580 (vtables 0x01275ABC / 0x01275AE4 / 0x01275B0C slot 0), whose RTTI base-class
//   arrays put CScriptModuleSelfTerminateBase directly under them (then CScriptModuleBase); the SelfTerminateBase ??_G 0x00854410
//   expands the same body inline. A compiler-generated destructor stores no vtable of its own (GOTCHAS), so the only store is the
//   inlined base dtor's CScriptModuleBase vtable 0x01255F74. Name: Ego_r.pdb ??1CScriptModuleSelfTerminateBase@NCameraModes@@UAE@XZ.
//   The landed binding "??1CScriptModuleBase@NCameraModes@@QAE@XZ" (caller 0x00854500) is wrong (NAME_FIXES.tsv).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (twice).
// Layout: Ego_r.pdb CScriptModuleSelfTerminateBase (0x54) = CScriptModuleBase + WFDuration +0x4C, EndFrame +0x50 (no dtor members).
// Shape: an implicit destructor (no user ~ in the class); the qualified explicit call emits it out of line.
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
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
namespace NCameraModes {
class CScriptModuleSelfTerminateBase : public CScriptModuleBase {
public:
    CScriptModuleSelfTerminateBase(long duration);
    long WFDuration;
    long EndFrame;
};
}
void DestroyModule(NCameraModes::CScriptModuleSelfTerminateBase* p) { p->NCameraModes::CScriptModuleSelfTerminateBase::~CScriptModuleSelfTerminateBase(); }