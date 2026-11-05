// NCameraModes::CScriptModuleBase::`scalar deleting destructor'(unsigned int flags) -- run ~CScriptModuleBase inline (clear the owner and camera-manager pointers, destroy the two intelligent pointers) and free the module when bit 0 of the flags is set
// Leaf: ??_GCScriptModuleBase@NCameraModes@@UAEPAXI@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x005ed150, 55 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): vtable 0x01255F74 slot 0 (RTTI .?AVCScriptModuleBase@NCameraModes@@, rebuild/manifest/rtti-vtables.tsv); the body stores
//   the CScriptModuleBase vtable 0x01255F74 (the derived implicit destructors inline the base one without their own store).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (PThingOrigin +0x0C, then PCamPoint +0x04), operator
//   delete 0x00BFE9BC.
// Shape (family L019): Ego_r.pdb CScriptModuleBase members (bases from Fable.exe RTTI: every module derives directly from
//   CScriptModuleBase); `new` emits the vtable and the ??_G, the explicit `p->X::~X()` keeps VC7.1 from calling an
//   out-of-line ??1 from the ??_G (GOTCHAS, family pass 2).
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CThing;
#include "engine/C3DVector.h"
class CRightHandedSet {                                    // Ego_r.pdb, 24 bytes
public:
    C3DVector Up;                                         // +0x00
    C3DVector Forward;                                    // +0x0C
};
class CBase;
class CGameCameraManager;
namespace NCameraModes {
class CScriptModuleBase {                                  // Ego_r.pdb, 76 bytes (vptr +0x00)
public:
    CIntelligentPointer<CThing> PCamPoint;                // +0x04
    CIntelligentPointer<const CThing> PThingOrigin;       // +0x0C
    C3DVector CoordOrigin;                                // +0x14
    CRightHandedSet CoordAxis;                            // +0x20
    CBase* POwner;                                        // +0x38
    CGameCameraManager* PCameraManager;                   // +0x3C
    bool SelfTerminate;                                   // +0x40
    bool ShortenOnCollision;                              // +0x41
    long CutOutOverride;                                  // +0x44
    float FOVOverride;                                    // +0x48
    virtual ~CScriptModuleBase()                          // vtable 0x01255F74 slot 0 = its ??_G
    {
        POwner = 0;
        PCameraManager = 0;
    }
};
}
NCameraModes::CScriptModuleBase* MakeCScriptModuleBase() { return new NCameraModes::CScriptModuleBase; }
void KillCScriptModuleBase(NCameraModes::CScriptModuleBase* p) { p->NCameraModes::CScriptModuleBase::~CScriptModuleBase(); }