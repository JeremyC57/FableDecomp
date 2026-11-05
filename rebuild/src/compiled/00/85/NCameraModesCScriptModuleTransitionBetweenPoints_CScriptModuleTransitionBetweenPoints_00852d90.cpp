// NCameraModes::CScriptModuleTransitionBetweenPoints::CScriptModuleTransitionBetweenPoints(const CThing* thing, const C3DVector& startLookAt, const C3DVector& endLookAt, long frames, float fov) -- constructor: track the thing and set up a transition that stays at the thing's camera focal point
// Leaf: ??0CScriptModuleTransitionBetweenPoints
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00852D90, length 128 bytes (116 + int3 padding).
// Callees: NCameraModes::CScriptModuleBase::CScriptModuleBase() @ 0x005ED070,
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, ::SetPItem @ 0x00A01B90, ::GetPItem @ 0x00A01B50,
// CTCGraphicAppearance::GetCameraFocalPos(float) @ 0x004C12D0, CScriptModuleTransitionBetweenPoints::
// SetUpTransitionCamera @ 0x00851ED0 (six arguments).
// Vtables: 0x012759F4 (NCameraModes::CScriptModuleTransitionBetweenPoints), 0x01237F1C (CIntelligentPointer<const CThing>).
// Shape: PThing(thing) in the initialiser list; the body is the GoToLookAt ctor (0x00852D10) idiom -- the
// thing in a named local (GetPItem before the focal-point pushes), the focal point in a named frame local
// -- then SetUpTransitionCamera(focal, focal, startLookAt, endLookAt, frames, fov). `ret 14h`.
// Layout: PDB (0x90 bytes, see the landed overloads 0x008524C0 / 0x00852510). Parameter names are descriptive.
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

struct CScriptModuleBase {
    virtual void v00();
    unsigned char m_state[0x4c - 4];
    CScriptModuleBase();                             // 0x005ED070
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer_CThingConst(const CThing* thing) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)((const CBaseObject*)thing)); }
    const CThing* operator->() const { return (const CThing*)GetPItem(); }
};

namespace NCameraModes {

struct CScriptModuleTransitionBetweenPoints : CScriptModuleBase {   // vtable 0x012759F4
    CIntelligentPointer_CThingConst PThing;          // +0x4c
    C3DVector Increment;                             // +0x54
    C3DVector LookAtIncrement;                       // +0x60
    C3DVector LookAtPos;                             // +0x6c
    C3DVector StartPos;                              // +0x78
    long      EndFrame;                              // +0x84
    float     FOV;                                   // +0x88
    bool      PreventSelfTermination;                // +0x8c
    virtual void v00();
    void SetUpTransitionCamera(const C3DVector& startPos, const C3DVector& endPos, const C3DVector& startLookAt,
                               const C3DVector& endLookAt, long frames, float fov);   // 0x00851ED0
    CScriptModuleTransitionBetweenPoints(const CThing* thing, const C3DVector& startLookAt,
                                         const C3DVector& endLookAt, long frames, float fov);
};

CScriptModuleTransitionBetweenPoints::CScriptModuleTransitionBetweenPoints(const CThing* thing, const C3DVector& startLookAt,
                                                                           const C3DVector& endLookAt, long frames, float fov)
    : PThing(thing)
{
    const CThing* target = PThing.operator->();
    C3DVector focal = target->GetTCGraphicAppearance()->GetCameraFocalPos(1.0f);
    SetUpTransitionCamera(focal, focal, startLookAt, endLookAt, frames, fov);
}

}