// CInputTypeXboxPadRightStickEvent::IsEventOfThisType(const CInputEvent&) const -- true when the event's type is INPUT_EVENT_TYPE_PAD_RIGHT_STICK (0x12)
// Leaf: IsEventOfThisType@CInputTypeXboxPadRightStickEvent
// Retail: Fable.exe @ 0x0066F390, 20 bytes, vtable 0x012317F0 slot 3; label PDB-confirmed (labels_rtti_port).
// Callee: CInputEvent::GetType() const @ 0x00A03B40 (`mov eax,[ecx+0x28] ; ret`, out of line; identical-code-folded with the
// catalog's CWorld::DrawGetEnvironment). Ego_r.pdb CInputEvent: Type (EInputEventType) at +0x28. The enumerator name is ours.
// Layout: engine/CInputTypeXboxPadRightStickEvent.h; no member is read.
enum EInputEventType { INPUT_EVENT_TYPE_PAD_RIGHT_STICK = 0x12, INPUT_EVENT_TYPE_FORCE_DWORD = 0x7fffffff };
class CInputEvent {
public:
    EInputEventType GetType() const;                     // 0x00A03B40
};
class CInputTypeXboxPadRightStickEvent {
public:
    virtual void _v00();
    virtual void _v01();
    virtual void _v02();
    virtual bool IsEventOfThisType(const CInputEvent& event) const;          // slot 3
};

bool CInputTypeXboxPadRightStickEvent::IsEventOfThisType(const CInputEvent& event) const
{
    return event.GetType() == INPUT_EVENT_TYPE_PAD_RIGHT_STICK;
}