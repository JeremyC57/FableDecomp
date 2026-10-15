// NCameraModes::CScriptModuleTransitionBetweenPoints::CScriptModuleTransitionBetweenPoints(const C3DVector& startPos, const C3DVector& endPos, const C3DVector& startLookAt, const C3DVector& endLookAt, long frames, float fov) -- constructor: SetUpTransitionCamera with all six arguments
// Leaf: ??0CScriptModuleTransitionBetweenPoints
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00852510, length 75 bytes.
// Callees: CScriptModuleBase::CScriptModuleBase() @ 0x005ED070, CBaseIntelligentPointer::CBaseIntelligentPointer()
// @ 0x00A01B10, CScriptModuleTransitionBetweenPoints::SetUpTransitionCamera @ 0x00851ED0.
// Vtables: 0x012759F4 (NCameraModes::CScriptModuleTransitionBetweenPoints), 0x01237F1C (CIntelligentPointer<const CThing>).
// Shape: as the five-argument overload 0x008524C0, the six arguments forwarded in order (loaded three at
// a time through eax/ecx/edx, pushed right to left). `ret 18h`.
// Layout: see 0x008524C0. Parameter names are descriptive.
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
