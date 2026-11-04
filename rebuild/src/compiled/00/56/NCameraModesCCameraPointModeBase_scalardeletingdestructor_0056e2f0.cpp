// NCameraModes::CCameraPointModeBase::`scalar deleting destructor'(unsigned int) -- its vtable, ~PCameraPoint, the camera-mode base destructor, free for flag bit 0
// Leaf: ??_GCCameraPointModeBase
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe 0x0056E2F0, 49 bytes; NCameraModes::CCameraPointModeBase vtable 0x0124DDDC slot 0
// (rtti-vtables.tsv). The manifest label "CThingTrackNode::`vector_deleting_destructor'" is propagated.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (+0xa4), the camera-mode base
// destructor 0x0056E0E0 (labelled "~CTCActionUseBed", propagated), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb PCameraPoint +0xc8 sits at +0xa4 in retail (the base is 0x24 bytes narrower); the
// base class is RTTI's NCameraModes::CBase (namepass CAM1; was the descriptive CCameraModeBase). Local model.
// Shape: a user (empty) inline virtual destructor stores the vtable (scheduled after the member's `lea`);
// the intelligent pointer's generated destructor calls the base destructor directly; then the out-of-line
// base destructor; a `new` factory pulls the `??_G` into the TU.
class CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* PItem;                                         // +0x04
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                          // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing const>
    virtual void v00();
};

#include "engine/C3DVector.h"

namespace NCameraModes {

struct CBase {
    CBase();
    virtual ~CBase();                          // 0x0056E0E0
    unsigned char _body[0xa4 - 4];
};

struct CCameraPointModeBase : CBase {
    CIntelligentPointer PCameraPoint;                    // +0xa4
    C3DVector           Pos;                             // +0xac
    float               FOV;                             // +0xb8
    bool                CutInto;                         // +0xbc
    bool                CutOutOf;                        // +0xbd
    virtual ~CCameraPointModeBase() {}
};

} // namespace NCameraModes

NCameraModes::CCameraPointModeBase* NewCameraPointModeBase()
{
    return new NCameraModes::CCameraPointModeBase;
}