// NCameraModes::CScriptModuleTransitionBetweenPoints::CScriptModuleTransitionBetweenPoints(const C3DVector& pos, const C3DVector& startLookAt, const C3DVector& endLookAt, long frames, float fov) -- constructor for a transition that stays at one position: SetUpTransitionCamera(pos, pos, ...)
// Leaf: ??0CScriptModuleTransitionBetweenPoints
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008524C0, length 71 bytes.
// Callees: CScriptModuleBase::CScriptModuleBase() @ 0x005ED070, CBaseIntelligentPointer::CBaseIntelligentPointer()
// @ 0x00A01B10, CScriptModuleTransitionBetweenPoints::SetUpTransitionCamera @ 0x00851ED0 (six arguments).
// Vtables: 0x012759F4 (NCameraModes::CScriptModuleTransitionBetweenPoints), 0x01237F1C (CIntelligentPointer<const CThing>).
// Shape: PThing (+0x4c) default-constructed with NO SetPItem (base call + vtable only; the vtable store
// is scheduled down to just before the set-up call); the body forwards the first argument twice
// (`push eax ; push eax`), the remaining four after it, right to left. `ret 14h` = five arguments.
// Layout (PDB, 0x90 bytes): PThing +0x4c, Increment +0x54, LookAtIncrement +0x60, LookAtPos +0x6c,
// StartPos +0x78, EndFrame +0x84, FOV +0x88, PreventSelfTermination +0x8c. Parameter names are descriptive.
struct CBaseObject;
struct C3DVector { float X, Y, Z; };

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                       // 0x00A01B10
};

struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer_CThingConst() : CBaseIntelligentPointer() { }
};

struct CScriptModuleBase {
    virtual void v00();
    unsigned char m_state[0x4c - 4];
    CScriptModuleBase();                             // 0x005ED070
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
    CScriptModuleTransitionBetweenPoints(const C3DVector& pos, const C3DVector& startLookAt, const C3DVector& endLookAt,
                                         long frames, float fov);
};

CScriptModuleTransitionBetweenPoints::CScriptModuleTransitionBetweenPoints(const C3DVector& pos, const C3DVector& startLookAt,
                                                                           const C3DVector& endLookAt, long frames, float fov)
{
    SetUpTransitionCamera(pos, pos, startLookAt, endLookAt, frames, fov);
}

}
