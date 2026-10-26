// CJoystick::UpdateMaintainedEventPositions() -- re-stamp every maintained (held) joystick event with the stick's current position, keeping its button, value and start time
// Leaf: UpdateMaintainedEventPositions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E42E0, length 122 bytes.
// Callees: the joystick's virtual stick-axis getters (slot 3 +0xc, slot 4 +0x10; implemented by CJoystickDX), and on each
// event: CInputEvent::GetJoystickButton @ 0x00A03B90 (`movzx eax,byte ptr [ecx+8]`; PDB ?GetJoystickButton@CInputEvent@@QBEJXZ
// returns LONG -- declaring it `unsigned char` moved `mov ecx,ebx` above `push eax`), GetJoystickButtonPressureFraction @
// 0x00A03BA0 (`fld [ecx+0Ch]`), and the joystick-button setter @ 0x00A040D0 (ret 14h: long, float, const C2DVector&, double;
// it stamps event type 0x14 between its siblings 0x00A04090 (0x13) and 0x00A04110 (0x15), so the "held" variant
// SetAsJoystickButtonHeld of the PDB's Pressed/Held/Released trio).
// Labels: the manifest's "CInputEvent::SetAsJoystickPosition2" for 0x00A040D0 is wrong (SetAsJoystickPosition is 0x00A04060, ret 0Ch,
// type 0x12); "CTCInventoryBase::GetCanItemBeQuickAssigned" / "CMap::DrawGetSeaLevel" for the two getters are propagated.
// Layout: engine/CJoystick.h offsets (MaintainedEventsList, an STLport list, at +0xd0c), modelled locally because the header
//   carries the vptr as a plain word and the list as opaque bytes (the slot 3/4 virtual calls and the node walk need types); CMaintainedEvent (Ego_r.pdb, 56 bytes) holds
// its CInputEvent at +4, so a node (next, prev, value) carries the event at +0xc and its StartTime (+0x30) at +0x3c.
// Shape: the event re-addressed through the node at every use (a reference local moves the registers); `C2DVector position(GetStickX(), GetStickY())` evaluates slot 4 first (right to left) and copies it; the call's
// arguments evaluate right to left (the double start time, the position, then the two getters); the list end is
// re-read every iteration; the double argument gives the 8-aligned frame.
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class CInputEvent {
public:
    unsigned char _data[0x30];
    float         StartTime;                       // +0x30
    long          GetJoystickButton() const;       // 0x00A03B90 (PDB ?GetJoystickButton@CInputEvent@@QBEJXZ)
    float         GetJoystickButtonPressureFraction() const;   // 0x00A03BA0
    void SetAsJoystickButtonHeld(long button, float pressure, const C2DVector& position, double time);   // 0x00A040D0 (event type 0x14)
};
class CMaintainedEvent { public:
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
class CInputManager;
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
        node->m_data.Event.SetAsJoystickButtonHeld(node->m_data.Event.GetJoystickButton(),
                                                   node->m_data.Event.GetJoystickButtonPressureFraction(), position,
                                                   node->m_data.Event.StartTime);
    }
}