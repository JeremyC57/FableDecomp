// NUISystem::CManager::IsKeyHeld(EUsedKeys key) -- unless UI input is locked (+0x30), ask the input manager whether a key-down event for the engine key mapped to this UI key is queued (state 2 filter), then drop that event from the queue; false while locked
// Leaf: IsKeyHeld
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041E10D, length 97 bytes. Frame-pointer size TU: `#pragma optimize("y", off)` + `("s", on)`.
// Callees: CInputEvent::CInputEvent @ 0x00A04410 (manifest label CDefaultEngineMemoryInit is a byte-twin), the key map's operator[]
//   @ 0x0042D201 (map<EUsedKeys, long>), CInputManager::IsKeyboardEventInQueue(EInputEventType, EInputKey, CInputEvent*) @ 0x009F50A0,
//   CInputManager::RemoveEventFromQueue @ 0x009F4F70, ~CInputEvent @ 0x00A03B30.
// Layout (retail NUISystem::CManager): input manager +0x20, key map +0x24, input-locked word +0x30; CInputEvent is a 0x34-byte frame local
//   with its state word at +0x20.
// Shape: the input manager is a named local loaded BEFORE the state store and the map lookup (edi), the result parked in bl across the
//   removal and the event destructor; the locked arm (`xor al,al`) sits at the end, so the lookup is the nested `if`.
#pragma optimize("y", off)
#pragma optimize("s", on)
enum EInputEventType { EIET_KEY_DOWN = 1 };
enum EInputKey { EIK_NONE = 0 };
class CInputEvent { public:
    unsigned char _pad00[0x20];
    long          m_state;                         // +0x20
    unsigned char _pad24[0x10];
    CInputEvent();                                 // 0x00A04410
    ~CInputEvent();                                // 0x00A03B30
};
class CInputManager { public:
    bool IsKeyboardEventInQueue(EInputEventType type, EInputKey key, CInputEvent* event);   // 0x009F50A0
    void RemoveEventFromQueue(CInputEvent* event);                                          // 0x009F4F70
};
namespace NUISystem {
class CManager { public:
    enum EUsedKeys { EUK_NONE };
    struct CKeyMap {
        long& operator[](const EUsedKeys& key);    // 0x0042D201
        unsigned char _d[8];
    };
    unsigned char  _pad00[0x20];
    CInputManager* m_pInputManager;                // +0x20
    CKeyMap        m_keyMap;                       // +0x24
    unsigned char  _pad2c[4];
    long           m_inputLocked;                  // +0x30
    bool IsKeyHeld(EUsedKeys key);
};
bool CManager::IsKeyHeld(EUsedKeys key)
{
    if (m_inputLocked == 0) {
        CInputEvent event;
        CInputManager* input = m_pInputManager;
        event.m_state = 2;
        bool held = input->IsKeyboardEventInQueue(EIET_KEY_DOWN, (EInputKey)m_keyMap[key], &event);
        m_pInputManager->RemoveEventFromQueue(&event);
        return held;
    }
    return false;
}
}