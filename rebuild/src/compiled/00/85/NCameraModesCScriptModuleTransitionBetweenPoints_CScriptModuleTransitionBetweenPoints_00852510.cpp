// NCameraModes::CScriptModuleTransitionBetweenPoints::CScriptModuleTransitionBetweenPoints(const C3DVector& startPos, const C3DVector& endPos, const C3DVector& startLookAt, const C3DVector& endLookAt, long frames, float fov) -- constructor: SetUpTransitionCamera with all six arguments
// Leaf: ??0CScriptModuleTransitionBetweenPoints
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00852510, length 75 bytes.
// Callees: CScriptModuleBase::CScriptModuleBase() @ 0x005ED070, CBaseIntelligentPointer::CBaseIntelligentPointer()
// @ 0x00A01B10, CScriptModuleTransitionBetweenPoints::SetUpTransitionCamera @ 0x00851ED0.
// Vtables: 0x012759F4 (NCameraModes::CScriptModuleTransitionBetweenPoints), 0x01237F1C (CIntelligentPointer<const CThing>).
// Shape: as the five-argument overload 0x008524C0, the six arguments forwarded in order (loaded three at
// a time through eax/ecx/edx, pushed right to left). `ret 18h`.
// Layout: see 0x008524C0. Parameter names are descriptive.
class CBaseObject;
#include "engine/C3DVector.h"

class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CThing;

struct CScriptModuleBase {
    virtual void v00();
    unsigned char m_state[0x4c - 4];
    CScriptModuleBase();                             // 0x005ED070
};

namespace NCameraModes {

struct CScriptModuleTransitionBetweenPoints : CScriptModuleBase {   // vtable 0x012759F4
    CIntelligentPointer<const CThing> PThing;          // +0x4c
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
    CScriptModuleTransitionBetweenPoints(const C3DVector& startPos, const C3DVector& endPos, const C3DVector& startLookAt,
                                         const C3DVector& endLookAt, long frames, float fov);
};

CScriptModuleTransitionBetweenPoints::CScriptModuleTransitionBetweenPoints(const C3DVector& startPos, const C3DVector& endPos,
                                                                           const C3DVector& startLookAt, const C3DVector& endLookAt,
                                                                           long frames, float fov)
{
    SetUpTransitionCamera(startPos, endPos, startLookAt, endLookAt, frames, fov);
}

}