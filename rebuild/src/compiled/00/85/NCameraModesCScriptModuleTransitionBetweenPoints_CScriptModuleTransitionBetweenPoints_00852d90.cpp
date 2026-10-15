// NCameraModes::CScriptModuleTransitionBetweenPoints::CScriptModuleTransitionBetweenPoints(const CThing* thing, const C3DVector& startLookAt, const C3DVector& endLookAt, long frames, float fov) -- constructor: track the thing and set up a transition that stays at the thing's camera focal point
// Leaf: ??0CScriptModuleTransitionBetweenPoints
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
struct CBaseObject;

struct C3DVector { float X, Y, Z; };

struct CTCGraphicAppearance {
    C3DVector GetCameraFocalPos(float t) const;      // 0x004C12D0
};

struct CThing {
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* m_pGraphicAppearance;      // +0x64
};

struct CScriptModuleBase {
    virtual void v00();
    unsigned char m_state[0x4c - 4];
    CScriptModuleBase();                             // 0x005ED070
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                       // 0x00A01B10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CBaseObject* item);          // 0x00A01B90
};

struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer_CThingConst(const CThing* thing) : CBaseIntelligentPointer() { SetPItem((const CBaseObject*)thing); }
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
    C3DVector focal = target->m_pGraphicAppearance->GetCameraFocalPos(1.0f);
    SetUpTransitionCamera(focal, focal, startLookAt, endLookAt, frames, fov);
}

}