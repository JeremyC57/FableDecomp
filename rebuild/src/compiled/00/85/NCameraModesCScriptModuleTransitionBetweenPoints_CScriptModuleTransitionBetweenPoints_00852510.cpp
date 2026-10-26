// NCameraModes::CScriptModuleTransitionBetweenPoints::CScriptModuleTransitionBetweenPoints(const C3DVector& startPos, const C3DVector& endPos, const C3DVector& startLookAt, const C3DVector& endLookAt, long frames, float fov) -- constructor: SetUpTransitionCamera with all six arguments
// Leaf: ??0CScriptModuleTransitionBetweenPoints
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00852510, length 75 bytes.
// Callees: CScriptModuleBase::CScriptModuleBase() @ 0x005ED070, CBaseIntelligentPointer::CBaseIntelligentPointer()
// @ 0x00A01B10, CScriptModuleTransitionBetweenPoints::SetUpTransitionCamera @ 0x00851ED0.
// Vtables: 0x012759F4 (NCameraModes::CScriptModuleTransitionBetweenPoints), 0x01237F1C (CIntelligentPointer<const CThing>).
// Shape: as the five-argument overload 0x008524C0, the six arguments forwarded in order (loaded three at
// a time through eax/ecx/edx, pushed right to left). `ret 18h`.
// Layout: see 0x008524C0. Parameter names are descriptive.
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