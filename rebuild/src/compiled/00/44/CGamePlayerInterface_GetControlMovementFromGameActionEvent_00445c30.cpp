// CGamePlayerInterface::GetControlMovementFromGameActionEvent(EGameAction action, const CInputEvent& event, C2DVector& movement) -- walk the input types bound to the game action (the Controls map's equal range) and, for the first one that recognises the event, store its movement vector and return true; false when none does
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00445C30, length 122 bytes (+ int3 padding). `ret 0Ch`.
// Callees: CVectorMap<EGameAction, CCountedPointer<const CInputTypeBase> >::UpperBound @ 0x00447CE0 and ::LowerBound @ 0x00447C80 (key by the
// address of the action argument; the LowerBound label names another instantiation of the identical body), the input type's virtual
// slots 3 (+0xc, recognises the event) and 4 (+0x10, the movement, a C2DVector returned through a hidden pointer).
// Layout: engine/CGamePlayerInterface.h (Ego_r.pdb): Controls at +0x7a8 (the retail offset agrees); a map entry is
// {EGameAction, CCountedPointer<const CInputTypeBase>} = 12 bytes, the input type object at +4.
// Shape: the upper bound is taken first (kept in ebx), then the lower bound walks to it; the input type object is a named local per entry (esi, used by
// both virtual calls; going through the entry twice re-reads it); the movement is the implicit two-dword copy of the returned vector.
class C2DVector { public:
    float X;
    float Y;
};

class CInputEvent;

class CInputTypeBase { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual bool RecognisesEvent(const CInputEvent& event);         // slot 3 (+0x0c)
    virtual C2DVector GetMovement(const CInputEvent& event);        // slot 4 (+0x10)
};

struct SCountBlock;
struct CCountedPointer_InputType {
    CInputTypeBase* m_pObject;              // +0
    SCountBlock*    m_pCount;               // +4
    CInputTypeBase* operator->() const { return m_pObject; }
};

struct SControlEntry {                      // pair<EGameAction, CCountedPointer<const CInputTypeBase> >
    long                      first;        // +0x00
    CCountedPointer_InputType second;       // +0x04
};

struct CControlsMap {                       // CVectorMap<EGameAction, CCountedPointer<const CInputTypeBase>, ...>
    unsigned char _body[0x14];
    SControlEntry* UpperBound(const long* key);   // 0x00447CE0
    SControlEntry* LowerBound(const long* key);   // 0x00447C80
};

class CGamePlayerInterface { public:
    unsigned char _pad[0x7a8];
    CControlsMap  Controls;                 // +0x7a8
    bool GetControlMovementFromGameActionEvent(long action, const CInputEvent& event, C2DVector& movement);
};

bool CGamePlayerInterface::GetControlMovementFromGameActionEvent(long action, const CInputEvent& event, C2DVector& movement)
{
    SControlEntry* end = Controls.UpperBound(&action);
    for (SControlEntry* it = Controls.LowerBound(&action); it != end; ++it) {
        CInputTypeBase* inputType = it->second.m_pObject;
        if (inputType->RecognisesEvent(event)) {
            movement = inputType->GetMovement(event);
            return true;
        }
    }
    return false;
}