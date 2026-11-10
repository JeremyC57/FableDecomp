// NCameraModes::CScriptModuleUseLamp::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor: the implicit ~CScriptModuleUseLamp expanded (~CBaseIntelligentPointer on PLamp +0x5C and PCreature +0x54, then the out-of-line ~CScriptModuleSelfTerminateBase 0x00854310), then operator delete when bit 0 of the flags is set
// Leaf: ??_GCScriptModuleUseLamp@NCameraModes@@UAEPAXI@Z
// Retail: Fable.exe @ 0x00854540. Identity: slot 0 of vtable 0x01275AE4, RTTI .?AVCScriptModuleUseLamp@NCameraModes@@ : CScriptModuleSelfTerminateBase :
//   CScriptModuleBase (ghidra_out/rtti_bases.tsv; the catalog name agrees).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (Ego_r.pdb / FableWin.pdb ??1CBaseIntelligentPointer@@UAE@XZ),
//   NCameraModes::CScriptModuleSelfTerminateBase::~CScriptModuleSelfTerminateBase 0x00854310 (bound by the landed rows), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb NCameraModes::CScriptModuleBase (76 bytes: PCamPoint +4, PThingOrigin +0xC, CoordOrigin +0x14, CoordAxis +0x20,
//   POwner +0x38, PCameraManager +0x3C, SelfTerminate +0x40, ShortenOnCollision +0x41, CutOutOverride +0x44, FOVOverride +0x48),
//   CScriptModuleSelfTerminateBase (WFDuration +0x4C, EndFrame +0x50) and CScriptModuleUseLamp (PCreature +0x54, PLamp +0x5C, ...; 160 bytes); no
//   STL members, so retail keeps the Ego_r offsets (the deleting destructor's +0x54 / +0x5C agree). No engine header models these
//   classes, so they are declared here over the shared engine/CIntelligentPointer.h, engine/C3DVector.h and engine/CRightHandedSet.h.
//   CScriptModuleBase::POwner's pointee class (Ego_r `CBase *`) is kept as void*. The derived class declares no destructor.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
class CThing;
class CGameCameraManager;
namespace NCameraModes {
class CScriptModuleBase {
public:
    CScriptModuleBase();                                                // Ego_r.pdb ??0CScriptModuleBase@NCameraModes@@QAE@XZ
    virtual ~CScriptModuleBase();
    CIntelligentPointer<CThing> PCamPoint;                              // +0x04
    CIntelligentPointer<const CThing> PThingOrigin;                     // +0x0C
    C3DVector CoordOrigin;                                              // +0x14
    CRightHandedSet CoordAxis;                                          // +0x20
    void* POwner;                                                       // +0x38
    CGameCameraManager* PCameraManager;                                 // +0x3C
    bool SelfTerminate;                                                 // +0x40
    bool ShortenOnCollision;                                            // +0x41
    long CutOutOverride;                                                // +0x44
    float FOVOverride;                                                  // +0x48
};
class CScriptModuleSelfTerminateBase : public CScriptModuleBase {
public:
    CScriptModuleSelfTerminateBase(long duration);                      // Ego_r.pdb ??0CScriptModuleSelfTerminateBase@NCameraModes@@QAE@J@Z
    virtual ~CScriptModuleSelfTerminateBase();                          // 0x00854310
    virtual bool IsFinished() const;
    long WFDuration;                                                    // +0x4C
    long EndFrame;                                                      // +0x50
};
class CScriptModuleUseLamp : public CScriptModuleSelfTerminateBase {                   // vtable 0x01275AE4
public:
    CScriptModuleUseLamp(const CThing& creature, const CThing& lamp, long duration); // Ego_r.pdb ??0CScriptModuleUseLamp@NCameraModes@@QAE@ABVCThing@@0J@Z
    virtual void Update();
protected:
    virtual void OnInitLeaf();
public:
    CIntelligentPointer<const CThing> PCreature;                        // +0x54
    CIntelligentPointer<const CThing> PLamp;                            // +0x5C
    C3DVector Origin;                                                   // +0x64
    float Theta;                                                        // +0x70
    float Rad;                                                          // +0x74
    float Height;                                                       // +0x78
    float DeltaTheta;                                                   // +0x7C
    float DeltaRad;                                                     // +0x80
    float DeltaHeight;                                                  // +0x84
    C3DVector AxisX;                                                    // +0x88
    C3DVector AxisY;                                                    // +0x94
};
}
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CScriptModuleUseLamp) == 0xA0);

// Emission harness (not a retail function): an inline copy construction emits the vftable and its deleting destructor.
NCameraModes::CScriptModuleBase* EmitCScriptModuleUseLampDeletingDestructor(const NCameraModes::CScriptModuleUseLamp& module)
{
    return new NCameraModes::CScriptModuleUseLamp(module);
}
