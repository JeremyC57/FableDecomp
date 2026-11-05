// NCameraModes::CScriptModuleGoToLookAt::Update() -- put the owning camera at GoTo; while the tracked thing is alive refresh LookAt from its camera focal point; aim the camera at LookAt
// Leaf: Update
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008534F0, length 144 bytes (141 + int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times, re-fetched per test),
// CTCGraphicAppearance::GetCameraFocalPos(float) @ 0x004C12D0 (C3DVector by hidden pointer).
// Shape: position copy first (implicit copy through re-based pointers); nested null / dead-bit (+0x91)
// tests; then the ctor 0x00852D10 idiom: the thing in a named local (GetPItem before the pushes), the
// focal point in a named local re-read from the frame, LookAt assigned; finally the owner's look-at.
// Layout: PDB NCameraModes::CScriptModuleGoToLookAt (0x6c bytes): PThing +0x4c, GoTo +0x54, LookAt +0x60,
// over the 0x4c-byte CScriptModuleBase (POwner +0x38).
class CBaseObject;

#include "engine/C3DVector.h"

namespace NGraphicAppearance {
struct CTCGraphicAppearance {
    C3DVector GetCameraFocalPos(float t) const;      // 0x004C12D0
};
}
using NGraphicAppearance::CTCGraphicAppearance;

#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"

struct CCameraState {
    unsigned char _pad00[0x10];
    C3DVector     LookAt;                            // +0x10
    unsigned char _pad1c[0x38];
    C3DVector     Position;                          // +0x54
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    const CThing* operator->() const { return (const CThing*)GetPItem(); }
};

namespace NCameraModes {

struct CScriptModuleGoToLookAt {
    virtual void v00();
    unsigned char                   _pad04[0x34];
    CCameraState*                   POwner;          // +0x38
    unsigned char                   _pad3c[0x10];
    CIntelligentPointer_CThingConst PThing;          // +0x4c
    C3DVector                       GoTo;            // +0x54
    C3DVector                       LookAt;          // +0x60
    void Update();
};

void CScriptModuleGoToLookAt::Update()
{
    POwner->Position = GoTo;
    if (PThing.GetPItem()) {
        if (!PThing->IsDead()) {
            const CThing* target = PThing.operator->();
            C3DVector focal = target->GetTCGraphicAppearance()->GetCameraFocalPos(1.0f);
            LookAt = focal;
        }
    }
    POwner->LookAt = LookAt;
}

}