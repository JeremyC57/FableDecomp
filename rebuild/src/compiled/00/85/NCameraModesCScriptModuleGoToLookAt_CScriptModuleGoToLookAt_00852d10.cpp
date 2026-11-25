// NCameraModes::CScriptModuleGoToLookAt::CScriptModuleGoToLookAt(const C3DVector& goTo, const CThing* thing) -- constructor: track the thing, go to goTo and look at the thing's camera focal point
// Leaf: ??0CScriptModuleGoToLookAt
// Naming (NF450 gen, 2026-11-25): callee-only respell of this row's base / member construct calls to the PDB constructor spelling
//   (T1 tracker; target self rows' Leaf): the row's own bodiless model ctor takes the PDB parameter types, call sites pass *x:
//   0x005ED070 ??0CScriptModuleBase@NCameraModes@@QAE@XZ (was ??0CScriptModuleBase@@QAE@XZ); the model class is wrapped in namespace NCameraModes (PDB scope) with a using-declaration; the ctor is reached by implicit base initialisation (no call-site edit); r449a
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00852D10, length 128 bytes (125 + int3 padding).
// Callees: NCameraModes::CScriptModuleBase::CScriptModuleBase() @ 0x005ED070,
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, ::SetPItem @ 0x00A01B90, ::GetPItem @ 0x00A01B50,
// CTCGraphicAppearance::GetCameraFocalPos(float) @ 0x004C12D0 (C3DVector by hidden pointer).
// Vtables: 0x012657D8 (NCameraModes::CScriptModuleGoToLookAt), 0x01237F1C (CIntelligentPointer<const CThing>).
// Shape: initialiser list PThing(thing) (base ctor, vtable, SetPItem) and GoTo(goTo) through the
// memberwise user copy ctor (direct stores to +0x54..); the body takes the thing into a named local
// (retail calls GetPItem BEFORE pushing the focal-point arguments) and the by-value focal point into a
// named local (re-read from the frame, not from eax), then assigns LookAt with the implicit copy through
// `lea edx,[esi+60h]`. The thing's graphic-appearance component is cached at +0x64.
// Layout: PDB NCameraModes::CScriptModuleGoToLookAt (0x6c bytes): PThing +0x4c, GoTo +0x54, LookAt +0x60;
// the landed two-vector overload 0x007187C0 uses the same model.
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

namespace NCameraModes { struct CScriptModuleBase {
    virtual void v00();
    unsigned char m_state[0x4c - 4];
    CScriptModuleBase();                             // 0x005ED070
}; } using NCameraModes::CScriptModuleBase;

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer_CThingConst(const CThing* thing) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)((const CBaseObject*)thing)); }
    const CThing* operator->() const { return (const CThing*)GetPItem(); }
};

namespace NCameraModes {

struct CScriptModuleGoToLookAt : CScriptModuleBase {     // vtable 0x012657D8
    CIntelligentPointer_CThingConst PThing;          // +0x4c
    C3DVector                       GoTo;            // +0x54
    C3DVector                       LookAt;          // +0x60
    virtual void v00();
    CScriptModuleGoToLookAt(const C3DVector& goTo, const CThing* thing);
};

CScriptModuleGoToLookAt::CScriptModuleGoToLookAt(const C3DVector& goTo, const CThing* thing)
    : PThing(thing), GoTo(goTo)
{
    const CThing* target = PThing.operator->();
    C3DVector focal = target->GetTCGraphicAppearance()->GetCameraFocalPos(1.0f);
    LookAt = focal;
}

}