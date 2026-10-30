// CMouse::UpdateMaintainedEvents() -- re-issue every maintained (held) mouse-button event with the drag movement since the press and the current time
// Leaf: UpdateMaintainedEvents
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A66C20, length 688 bytes (incl. a 5-entry jump table + 27-entry byte index table), `ret`.
// Callees: CMouse::GetPos (virtual slot 2, +8; CMouseDX's is `lea eax,[ecx+3414h]` @ 0x00AB5F00), CInputEvent::GetType @ 0x00A03B40
//          (labelled "CWorld::DrawGetEnvironment", propagated), CInputEvent::GetMousePos @ 0x00A04380 (C2DVector of +0x18/+0x1c by hidden
//          pointer; labelled "CMessageEventFilter_IsOriginatedByThing", propagated), CInputEvent::SetAsLeft/Right/MiddleMouseButtonHeld
//          @ 0x00A03D10 / 0x00A03DC0 / 0x00A03E70, SetAsExtraMouseButtonHeld @ 0x00A03F20, GetTickCount (import 0x0143FDFC = KERNEL32.dll!GetTickCount, held in ebx).
// Globals: 4294967296.0f @ 0x0122DCB4, 0.001f @ 0x01266300.
// Layout: as the landed CKeyboard::UpdateMaintainedEvents 0x00A66880: the list head pointer at +0x340c, node {next, prev, CMaintainedEvent}
//   with the CInputEvent at node+0xc; CInputEvent (PDB 0x34): Type +0x28, CurrentTime +0x2c, StartTime +0x30.
// Shape: `and esp,-8` frame for the double locals; the zeroed C3DVector movement and the two GetPos reads (member-wise integer copies
//   into a default-constructed C2DVector) sit before the loop; the start time is read through the node (`fld [edi+3Ch]`), the rest
//   through the event reference (esi).
class C2DVector { public:
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

enum EInputEventType {
    INPUT_EVENT_LMB_HELD = 5,
    INPUT_EVENT_RMB_HELD = 8,
    INPUT_EVENT_MMB_HELD = 11,
    INPUT_EVENT_EXTRA_BUTTON_HELD_0 = 27,
    INPUT_EVENT_EXTRA_BUTTON_HELD_1,
    INPUT_EVENT_EXTRA_BUTTON_HELD_2,
    INPUT_EVENT_EXTRA_BUTTON_HELD_3,
    INPUT_EVENT_EXTRA_BUTTON_HELD_4
};

class CInputEvent { public:                                // 0x34 bytes (PDB)
    unsigned char _body[0x28];
    long          Type;                            // +0x28
    float         CurrentTime;                     // +0x2c
    float         StartTime;                       // +0x30
    long GetType() const;                          // 0x00A03B40
    C2DVector GetMousePos() const;                 // 0x00A04380
    void SetAsLeftMouseButtonHeld(const C2DVector& pos, const C3DVector& movement, double startTime, double currentTime);   // 0x00A03D10
    void SetAsRightMouseButtonHeld(const C2DVector& pos, const C3DVector& movement, double startTime, double currentTime);  // 0x00A03DC0
    void SetAsMiddleMouseButtonHeld(const C2DVector& pos, const C3DVector& movement, double startTime, double currentTime); // 0x00A03E70
    void SetAsExtraMouseButtonHeld(const C2DVector& pos, const C3DVector& movement, double startTime, double currentTime, long button); // 0x00A03F20
};
class CMaintainedEvent { public:
    long        m_0;
    CInputEvent Event;                             // +0x04
};
struct SMaintainedEventNode {
    SMaintainedEventNode* m_pNext;
    SMaintainedEventNode* m_pPrev;
    CMaintainedEvent      m_value;                 // +0x08 (the event at +0x0c)
};
extern "C" __declspec(dllimport) unsigned long __stdcall GetTickCount(void);   // import 0x0143FDFC = KERNEL32.dll!GetTickCount

class CMouse {
public:
    virtual ~CMouse();
    virtual const C2DVector& GetScreenPos() const;
    virtual const C2DVector& GetPos() const;       // slot 2 (+8)
    CInputEvent           EventStore[256];         // +0x04
    long                  NoStoredEvents;          // +0x3404
    long                  NoEventScansRunning;     // +0x3408
    SMaintainedEventNode* m_pMaintainedEvents;     // +0x340c list<CMaintainedEvent> head
    void UpdateMaintainedEvents();
};

void CMouse::UpdateMaintainedEvents()
{
    C3DVector movement(0.0f, 0.0f, 0.0f);
    C2DVector pos;
    pos.X = GetPos().X;
    pos.Y = GetPos().Y;
    for (SMaintainedEventNode* node = m_pMaintainedEvents->m_pNext; node != m_pMaintainedEvents; node = node->m_pNext) {
        CInputEvent& event = node->m_value.Event;
        C2DVector startPos;
        double startTime;
        switch (event.GetType()) {
        case INPUT_EVENT_LMB_HELD:
            startPos = event.GetMousePos();
            movement.X = pos.X - startPos.X;
            movement.Y = pos.Y - startPos.Y;
            startTime = node->m_value.Event.StartTime;
            event.SetAsLeftMouseButtonHeld(pos, movement, startTime, (float)GetTickCount() * 0.001f);
            break;
        case INPUT_EVENT_RMB_HELD:
            startPos = event.GetMousePos();
            movement.X = pos.X - startPos.X;
            movement.Y = pos.Y - startPos.Y;
            startTime = node->m_value.Event.StartTime;
            event.SetAsRightMouseButtonHeld(pos, movement, startTime, (float)GetTickCount() * 0.001f);
            break;
        case INPUT_EVENT_MMB_HELD:
            startPos = event.GetMousePos();
            movement.X = pos.X - startPos.X;
            movement.Y = pos.Y - startPos.Y;
            startTime = node->m_value.Event.StartTime;
            event.SetAsMiddleMouseButtonHeld(pos, movement, startTime, (float)GetTickCount() * 0.001f);
            break;
        case INPUT_EVENT_EXTRA_BUTTON_HELD_0:
        case INPUT_EVENT_EXTRA_BUTTON_HELD_1:
        case INPUT_EVENT_EXTRA_BUTTON_HELD_2:
        case INPUT_EVENT_EXTRA_BUTTON_HELD_3:
        case INPUT_EVENT_EXTRA_BUTTON_HELD_4:
            startPos = event.GetMousePos();
            movement.X = pos.X - startPos.X;
            movement.Y = pos.Y - startPos.Y;
            startTime = node->m_value.Event.StartTime;
            event.SetAsExtraMouseButtonHeld(pos, movement, startTime, (float)GetTickCount() * 0.001f, event.GetType() - INPUT_EVENT_EXTRA_BUTTON_HELD_0);
            break;
        }
    }
}