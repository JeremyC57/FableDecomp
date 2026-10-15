// CJoystick::CJoystick(CInputManager* inputManager, bool exclusiveMode) -- base joystick constructor: vtable, the 64 stored input events, the owning input manager, an empty STLport list of maintained events, zero event counters and the exclusive-mode flag
// Leaf: ??0CJoystick
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E43B0, length 101 bytes.
// Label correction: the catalog's "CJoystickDX::Constructor" is the base class: the body stores CJoystick's vtable 0x0129C0EC
// and is called first by the CJoystickDX constructor @ 0x00AB7850 (which then stores 0x0129EA7C).
// Callees: CInputEvent::CInputEvent() @ 0x00A04410 (manifest "CDefaultEngineMemoryInit" is propagated), malloc @ 0x00BFEA0E
// (the 0x40-byte list sentinel: CMaintainedEvent is 56 bytes + the two links).
// Model: engine/CJoystick.h for EventStore (+4, 64 x 0x34), InputManager (+0xd04) and ExclusiveMode (+0xd08); retail's STLport
// list is one node pointer (+0xd0c), so NoStoredEvents / NoEventScansRunning sit at +0xd10 / +0xd14 (PDB +0xd18 / +0xd1c).
// Shape: the event array is the inlined `dec ebx ; jne` vector-constructor loop; the list is a member object whose inline
// constructor nulls the node, mallocs it and links it to itself; the flag is assigned in the body (stored last).
extern "C" void* __cdecl malloc(unsigned int size);
class CInputEvent {
public:
    unsigned char _data[0x34];
    CInputEvent();                                 // 0x00A04410
};
struct SMaintainedNode {                           // STLport list node of CMaintainedEvent (0x40 bytes)
    SMaintainedNode* m_pNext;
    SMaintainedNode* m_pPrev;
    unsigned char    m_data[0x38];
};
struct CMaintainedEventList {                      // STLport list<CMaintainedEvent>
    SMaintainedNode* m_pNode;
    CMaintainedEventList() : m_pNode(0)
    {
        SMaintainedNode* node = (SMaintainedNode*)malloc(sizeof(SMaintainedNode));
        node->m_pNext = node;
        node->m_pPrev = node;
        m_pNode = node;
    }
};
struct CInputManager;
class CJoystick {
public:
    virtual void v00();
    CInputEvent          EventStore[64];           // +0x04
    CInputManager*       InputManager;             // +0xd04
    bool                 ExclusiveMode;            // +0xd08
    CMaintainedEventList MaintainedEventsList;     // +0xd0c
    long                 NoStoredEvents;           // +0xd10
    long                 NoEventScansRunning;      // +0xd14
    CJoystick(CInputManager* inputManager, bool exclusiveMode);
};

CJoystick::CJoystick(CInputManager* inputManager, bool exclusiveMode)
    : InputManager(inputManager), NoStoredEvents(0), NoEventScansRunning(0)
{
    ExclusiveMode = exclusiveMode;
}
