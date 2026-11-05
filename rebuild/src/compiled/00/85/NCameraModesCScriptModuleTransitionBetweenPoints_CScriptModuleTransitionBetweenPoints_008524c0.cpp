// NCameraModes::CScriptModuleTransitionBetweenPoints::CScriptModuleTransitionBetweenPoints(const C3DVector& pos, const C3DVector& startLookAt, const C3DVector& endLookAt, long frames, float fov) -- constructor for a transition that stays at one position: SetUpTransitionCamera(pos, pos, ...)
// Leaf: ??0CScriptModuleTransitionBetweenPoints
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008524C0, length 71 bytes.
// Callees: CScriptModuleBase::CScriptModuleBase() @ 0x005ED070, CBaseIntelligentPointer::CBaseIntelligentPointer()
// @ 0x00A01B10, CScriptModuleTransitionBetweenPoints::SetUpTransitionCamera @ 0x00851ED0 (six arguments).
// Vtables: 0x012759F4 (NCameraModes::CScriptModuleTransitionBetweenPoints), 0x01237F1C (CIntelligentPointer<const CThing>).
// Shape: PThing (+0x4c) default-constructed with NO SetPItem (base call + vtable only; the vtable store
// is scheduled down to just before the set-up call); the body forwards the first argument twice
// (`push eax ; push eax`), the remaining four after it, right to left. `ret 14h` = five arguments.
// Layout (PDB, 0x90 bytes): PThing +0x4c, Increment +0x54, LookAtIncrement +0x60, LookAtPos +0x6c,
// StartPos +0x78, EndFrame +0x84, FOV +0x88, PreventSelfTermination +0x8c. Parameter names are descriptive.
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
    CScriptModuleTransitionBetweenPoints(const C3DVector& pos, const C3DVector& startLookAt, const C3DVector& endLookAt,
                                         long frames, float fov);
};

CScriptModuleTransitionBetweenPoints::CScriptModuleTransitionBetweenPoints(const C3DVector& pos, const C3DVector& startLookAt,
                                                                           const C3DVector& endLookAt, long frames, float fov)
{
    SetUpTransitionCamera(pos, pos, startLookAt, endLookAt, frames, fov);
}

}