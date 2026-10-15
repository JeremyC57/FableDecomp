// CJoystick::UpdateMaintainedEventPositions() -- re-stamp every maintained (held) joystick event with the stick's current position, keeping its button, value and start time
// Grade: DIFFER(122v122) -- scheduling only: the last call sets `mov ecx,ebx` before `push eax` (retail pushes the button first, then loads ecx)
// Leaf: UpdateMaintainedEventPositions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E42E0, length 122 bytes.
// Callees: the joystick's virtual stick-axis getters (slot 3 +0xc, slot 4 +0x10; implemented by CJoystickDX), and on each
// event: the button byte getter @ 0x00A03B90 (`movzx eax,byte ptr [ecx+8]`), the float value getter @ 0x00A03BA0
// (`fld [ecx+0Ch]`), CInputEvent::SetAsJoystickPosition2(unsigned char, float, const C2DVector&, double) @ 0x00A040D0
// (manifest labels for the two getters -- CTCInventoryBase::GetCanItemBeQuickAssigned / CMap::DrawGetSeaLevel -- are
// propagated).
// Model: engine/CJoystick.h (MaintainedEventsList, an STLport list, at +0xd0c); CMaintainedEvent (Ego_r.pdb, 56 bytes) holds
// its CInputEvent at +4, so a node (next, prev, value) carries the event at +0xc and its StartTime (+0x30) at +0x3c.
// Shape: the event re-addressed through the node at every use (a reference local moves the registers); `C2DVector position(GetStickX(), GetStickY())` evaluates slot 4 first (right to left) and copies it; the call's
// arguments evaluate right to left (the double start time, the position, then the two getters); the list end is
// re-read every iteration; the double argument gives the 8-aligned frame.
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CInputEvent {
public:
    unsigned char _data[0x30];
    float         StartTime;                       // +0x30
    unsigned char GetButton() const;               // 0x00A03B90
    float         GetValue() const;                // 0x00A03BA0
    void SetAsJoystickPosition2(unsigned char button, float value, const C2DVector& position, double time);   // 0x00A040D0
};
struct CMaintainedEvent {
    long        m_id;                              // +0x00
    CInputEvent Event;                             // +0x04
};
struct SMaintainedNode {                           // STLport list node
    SMaintainedNode* m_pNext;
    SMaintainedNode* m_pPrev;
    CMaintainedEvent m_data;                       // +0x08
};
struct CMaintainedEventList {                      // STLport list<CMaintainedEvent>: the sentinel node
    SMaintainedNode* m_pNode;
    SMaintainedNode* begin() const { return m_pNode->m_pNext; }
    SMaintainedNode* end() const { return m_pNode; }
};
struct CInputManager;
class CJoystick {
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual float GetStickX();                     // slot 3 (+0xc)
    virtual float GetStickY();                     // slot 4 (+0x10)
    unsigned char        EventStore[0xd00];        // +0x04 CInputEvent[64]
    CInputManager*       InputManager;             // +0xd04
    bool                 ExclusiveMode;            // +0xd08
    unsigned char        _pad_0xd09[3];
    CMaintainedEventList MaintainedEventsList;     // +0xd0c
    void UpdateMaintainedEventPositions();
};

void CJoystick::UpdateMaintainedEventPositions()
{
    for (SMaintainedNode* node = MaintainedEventsList.begin(); node != MaintainedEventsList.end(); node = node->m_pNext) {
        C2DVector position(GetStickX(), GetStickY());
        node->m_data.Event.SetAsJoystickPosition2(node->m_data.Event.GetButton(), node->m_data.Event.GetValue(), position,
                                                  node->m_data.Event.StartTime);
    }
}
