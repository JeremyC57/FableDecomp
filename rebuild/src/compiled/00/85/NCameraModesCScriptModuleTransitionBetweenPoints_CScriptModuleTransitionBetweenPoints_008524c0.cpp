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
class CBaseObject;
class C3DVector { public: float X, Y, Z; };

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01237F1C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

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