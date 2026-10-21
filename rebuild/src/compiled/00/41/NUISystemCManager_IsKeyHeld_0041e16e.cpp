// NUISystem::CManager::IsKeyHeld(EUsedKeys key) -- keyboard input only: is the key bound to this UI key held? (the queued event is consumed)
// Leaf: ?IsKeyHeld@CManager@NUISystem@@QAE_NW4EUsedKeys@12@@Z
// Retail: Fable.exe @ 0x0041E16E, 95 bytes, `ret 4`; Ego_r.pdb ?IsKeyHeld@CManager@NUISystem@@QAE_NW4EUsedKeys@12@@Z.
// Body: InputType (+0x30) != 0 -> false. Else a local CInputEvent (ctor 0x00A04410, dtor 0x00A03B30) with DeviceType = 2;
// held = PInputManager (+0x20)->IsKeyboardEventInQueue(2, Keys[key], event) (0x009F50A0; Keys the map<EUsedKeys, long>
// +0x24, operator[] 0x0042D201); PInputManager->RemoveEventFromQueue(event) (0x009F4F70); return held.
// Layout: retail NUISystem::CManager -- PInputManager +0x20, Keys +0x24 (12-byte STLport map), InputType +0x30
// (Ego_r.pdb +0x30 / +0x34 / +0x44; engine/CManager.h carries the PDB offsets); CInputEvent (Ego_r.pdb, 0x34 bytes):
// Data +0, DeviceType +0x20, DeviceNumber +0x24, Type +0x28, CurrentTime +0x2c, StartTime +0x30.
// Opt: size TU with frame pointer (`push ebp / mov ebp,esp`, `leave`, unaligned entry).
// Shape: PInputManager bound to a local after the event is built (retail keeps it in edi and shares the constant 2 in
// ebx); the keyboard path is the positive branch with `return false` after it.
#pragma optimize("s", on)
#pragma optimize("y", off)
enum EInputDeviceType { INPUT_DEVICE_TYPE_KEYBOARD = 2 };   // value as stored (name not confirmed)
enum EInputEventType { INPUT_EVENT_TYPE_KEY_HELD = 2 };     // value as pushed (name not confirmed)
enum EInputKey { INPUT_KEY_FORCE_DWORD = 0x7fffffff };
enum EInputType { INPUT_TYPE_KEYBOARD_AND_MOUSE = 0 };      // value as compared (name not confirmed)
class CInputEvent {
public:
    unsigned char    Data[0x20];                         // +0x00 UInputEventData
    EInputDeviceType DeviceType;                         // +0x20
    long             DeviceNumber;                       // +0x24
    EInputEventType  Type;                               // +0x28
    float            CurrentTime;                        // +0x2c
    float            StartTime;                          // +0x30
    CInputEvent();                                       // 0x00A04410
    ~CInputEvent();                                      // 0x00A03B30
};
class CInputManager {
public:
    bool IsKeyboardEventInQueue(EInputEventType type, EInputKey key, CInputEvent& event) const;   // 0x009F50A0
    void RemoveEventFromQueue(const CInputEvent& event);                                          // 0x009F4F70
};
namespace NUISystem {
class CManager {
public:
    enum EUsedKeys { USED_KEYS_FORCE_DWORD = 0x7fffffff };
    class CKeyMap {                                      // std::map<EUsedKeys, long> (12 bytes)
    public:
        void* _M_header;
        void* _M_node_count;
        void* _M_key_compare;
        long& operator[](const EUsedKeys& key);          // 0x0042D201
    };
    unsigned char  _pad00[0x20];
    CInputManager* PInputManager;                        // +0x20
    CKeyMap        Keys;                                 // +0x24
    EInputType     InputType;                            // +0x30
    bool IsKeyHeld(EUsedKeys key);
};
bool CManager::IsKeyHeld(EUsedKeys key)
{
    if (InputType == INPUT_TYPE_KEYBOARD_AND_MOUSE)
    {
        CInputEvent event;
        CInputManager* input = PInputManager;
        event.DeviceType = INPUT_DEVICE_TYPE_KEYBOARD;
        bool held = input->IsKeyboardEventInQueue(INPUT_EVENT_TYPE_KEY_HELD, (EInputKey)Keys[key], event);
        PInputManager->RemoveEventFromQueue(event);
        return held;
    }
    return false;
}}